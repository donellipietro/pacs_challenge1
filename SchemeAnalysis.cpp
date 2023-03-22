#include "SchemeAnalysis.hpp"

// Initialize members to optimize vector allocations
void SchemeAnalysis::init()
{
    n_ = N_ref_.size();
    errors_.reserve(n_);
    if (n_ > 1)
        conv_rates_.reserve(n_ - 1);
}

// Calculates the norm of the vector received as input
// using the norm chosen for the analysis.
// (Linf is the default choice)
double SchemeAnalysis::norm(std::vector<double> &x) const
{
    switch (norm_)
    {
    case Norms::Linf:
        return norm_Linf(x);

    case Norms::L1:
        return norm_L1(x);

    case Norms::L2:
        return norm_L2(x);

    default:
        return norm_Linf(x);
    }
}

// Computes the norm of the errors
double SchemeAnalysis::computeError() const
{
    std::vector<double> errors;
    errors.reserve(gett().size());

    for (std::size_t i = 0; i < gett().size(); ++i)
    {
        errors.push_back(uex_(gett()[i]) - getu()[i]);
    }

    /*
    std::cout << std::endl;
    std::cout << "errors = { ";
    for (auto vi : errors)
        std::cout << vi << ", ";
    std::cout << "}" << std::endl;
    std::cout << std::endl;
    */

    return norm(errors);
}

// Computes the order of the method
void SchemeAnalysis::computeOrder()
{
    std::cout << std::endl;
    std::cout << "########################" << std::endl;
    std::cout << "# Convergence analysis #" << std::endl;
    std::cout << "########################" << std::endl;
    std::cout << std::endl;

    if (!analysis_)
    {
        std::cout << "Attention! Analysys is not initialized for this solver." << std::endl
                  << std::endl;
        return;
    }

    performing_analysis_ = true;

    // Problema solution for different N
    for (unsigned int N_conv : N_ref_)
    {
        std::cout << "- Solving the problem for N = " << std::setw(4) << N_conv << " ... ";
        setN(N_conv);
        if (solve())
        {
            std::cout << "Solved! ";
            errors_.push_back(computeError());
            std::cout << "error = " << errors_.back() << std::endl;
        }
        else
        {
            std::cout << "Error, convergence analysis stopped." << std::endl;
            performing_analysis_ = false;
            restoreN();
            return;
        }
    }

    // Convergence rates computation
    std::cout << std::endl;
    std::cout << "Convergence rates: ";
    for (size_t i = 0; i < n_ - 1; ++i)
    {
        conv_rates_.push_back(std::log10(errors_[i + 1] / errors_[i]) / (std::log10(static_cast<double>(N_ref_[i]) / N_ref_[i + 1])));
        std::cout << conv_rates_.back() << " ";
    }
    std::cout << std::endl
              << std::endl;

    if (plots_)
        exportConvergence();

    // Restored the initial N
    performing_analysis_ = false;
    restoreN();
}

// Exports the information needed for the convergence plot
// (errors are normalized)
void SchemeAnalysis::exportConvergence() const
{
    std::ofstream fsolution("results/convergence.dat", std::ofstream::out);
    for (std::size_t i = 0; i < n_; ++i)
    {
        fsolution << static_cast<double>(N_ref_[0]) / N_ref_[i] << "\t"
                  << errors_[i] / errors_[0] << "\t"
                  << static_cast<double>(N_ref_[0]) / N_ref_[i] << "\t"
                  << std::pow(static_cast<double>(N_ref_[0]) / N_ref_[i], 2)
                  << std::endl;
    }
    fsolution.close();
}

// Computes the Linf norm of a given vector
double norm_Linf(const std::vector<double> &x)
{
    double result = 0.;

    for (const auto xi : x)
        result = std::max(result, std::abs(xi));
    return result;
    /*
    return std::abs(*std::max_element(x.begin(), x.end(),
                                      [](double a, double b)
                                      { return abs(a) < abs(b); }));
                                      */
}

// Computes the L1 norm of a given vector
double norm_L1(const std::vector<double> &x)
{
    double result = 0.;
    std::for_each(x.cbegin(), x.cend(),
                  [&result](const double xi)
                  { result += std::abs(xi); });
    return result;
}

// Computes the L2 norm of a given vector
double norm_L2(const std::vector<double> &x)
{
    double result = 0.;
    std::for_each(x.cbegin(), x.cend(),
                  [&result](const double xi)
                  { result += xi * xi; });
    return std::sqrt(result);
}
