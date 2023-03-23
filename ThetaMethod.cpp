#include "ThetaMethod.hpp"

// Initializes the numeric scheme
void ThetaMethod::initScheme()
{
    // Initialization of grid and solution vectors
    t_.clear();
    uh_.clear();
    t_.reserve(N_ + 1);
    uh_.reserve(N_ + 1);

    // Grid generation
    h_ = T_ / N_;
    for (std::size_t i = 0; i <= N_; ++i)
        t_.push_back(0 + i * h_);
}

// Initializes the model given the parameters
void ThetaMethod::init()
{
    std::cout << std::endl;
    std::cout << "########################" << std::endl;
    std::cout << "# Model initialization #" << std::endl;
    std::cout << "########################" << std::endl;
    std::cout << std::endl;

    if (!params_.sanityCheck())
        return;

    // Domain
    T_ = params_.domain.T;

    // Problem
    f_ = MuparserFun(params_.problem.f);
    y0_ = params_.problem.y0;

    // Scheme
    N_ = params_.scheme.N;
    theta_ = params_.scheme.theta;
    initScheme();

    // Analysis
    analysis_ = params_.analysis.analysis;
    uex_ = MuparserFun(params_.analysis.exact_solution);
    N_ref_ = params_.analysis.N_ref;
    norm_ = params_.analysis.norm;
    plots_ = params_.analysis.plots;
    SchemeAnalysis::init();

    std::cout << "Initialization completed!" << std::endl
              << std::endl;
}

// Solves the problem using the choosen theta-method
// returns true if the newton solver reaches convergence, false otherwise
bool ThetaMethod::solve()
{

    if (!performing_analysis_)
    {
        std::cout << std::endl;
        std::cout << "#######################" << std::endl;
        std::cout << "# Theta-Method solver #" << std::endl;
        std::cout << "#######################" << std::endl;
        std::cout << std::endl;
    }

    if (!params_.sanityCheck())
        return false;

    if (!performing_analysis_)
    {
        std::cout << "Problem: " << std::endl
                  << "y'(t) = " << params_.problem.f << " t in (0, " << T_ << ")" << std::endl
                  << "y(0) = " << y0_ << std::endl
                  << std::endl;

        std::cout << "Solver: " << std::endl
                  << "- theta-method with theta = " << theta_ << std::endl
                  << "- N = " << N_ << std::endl
                  << "- h = " << h_ << std::endl
                  << std::endl;
    }

    // Initilaization
    uh_.push_back(y0_);
    double un = uh_.front();
    double tn = t_.front();
    function1 F([this, &tn, &un](double x)
                { return x - theta_ * h_ * f_(tn + h_, x) - (1 - theta_) * h_ * f_(tn, un) - un; });
    function1 dF([&F](double x)
                 { return finiteDiff(F, x); });
    bool status_newton = true;

    // Iterative algorithm
    for (std::size_t i = 1; i <= N_; ++i)
    {
        if (theta_ == 0.)
            un = h_ * f_(tn, un) + un;
        else
            std::tie(un, status_newton) = apsc::Newton(F, dF, un);

        if (!status_newton)
        {
            std::cerr << "Newton algorithm cannot find the solution" << std::endl;
            return false;
        }
        else
        {
            uh_.push_back(un);
            tn = t_[i];
        }
    }

    if (!performing_analysis_)
    {
        std::cout << "Numerical solution: " << std::endl;
        printSolution();
        std::cout << std::endl;
        if (plots_)
            exportSolution();
    }

    return true;
}

// Prints the formatted solution in console
void ThetaMethod::printSolution() const
{
    std::cout << std::left << std::setw(8) << "t"
              << "\t"
              << "u" << std::endl;
    for (std::size_t i = 0; i <= N_; ++i)
    {
        std::cout << std::left << std::setw(8) << t_[i] << "\t" << uh_[i] << std::endl;
    }
}

// Exports the information needed for the solution plot
void ThetaMethod::exportSolution() const
{
    std::ofstream fsolution("results/solution.dat", std::ofstream::out);
    for (std::size_t i = 0; i <= N_; ++i)
    {
        fsolution << t_[i] << "\t" << uh_[i];
        if (analysis_)
            fsolution << "\t" << uex_(t_[i]);
        fsolution << std::endl;
    }
    fsolution.close();
}

// Updates N and re-initialize the scheme
void ThetaMethod::setN(unsigned int N)
{
    N_ = N;
    initScheme();
}

// Restore the initial N (used after the convergenge order conmputation)
void ThetaMethod::restoreN()
{
    setN(params_.scheme.N);
}

// Approximates the first derivative of a function with centered finite differences
double
finiteDiff(const function1 &f, const double x, const double h)
{
    return (f(x + h) - f(x - h)) / (2 * h);
}