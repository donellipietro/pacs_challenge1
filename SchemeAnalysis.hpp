#ifndef __SCHEME_ANALYSIS__

#define __SCHEME_ANALYSIS__

#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <functional>
#include <iomanip>

using function1 = std::function<double(double)>;
using function2 = std::function<double(double, double)>;

class SchemeAnalysis
{
protected:
    bool analysis_ = false;
    function1 u_ex_;
    std::vector<unsigned int> N_ref_;
    unsigned int n_;
    std::vector<double> errors_;
    std::vector<double> conv_rates_;
    std::function<double(std::vector<double>)> norm_;

public:
    SchemeAnalysis() = default;
    SchemeAnalysis(const function1 &u_ex,
                   const std::vector<unsigned int> &N_ref,
                   const std::function<double(std::vector<double>)> &norm)
        : analysis_(true),
          u_ex_(u_ex),
          N_ref_(N_ref),
          n_(N_ref.size()),
          norm_(norm)
    {
        errors_.reserve(n_);
        conv_rates_.reserve(n_ - 1);
    }

    virtual std::array<std::vector<double>, 2> getResult() const = 0;
    virtual bool solve() = 0;
    virtual void setN(const unsigned int N) = 0;
    virtual const std::vector<double> &gett() const = 0;
    virtual const std::vector<double> &getu() const = 0;

    double computeError() const
    {
        std::vector<double> errors;
        errors.reserve(gett().size());

        for (std::size_t i = 0; i < gett().size(); ++i)
        {
            errors.push_back(u_ex_(gett()[i]) - getu()[i]);
        }

        return norm_(errors);
    }

    void computeOrder()
    {

        std::cout << std::endl;
        std::cout << "########################" << std::endl;
        std::cout << "# Convergence analysis #" << std::endl;
        std::cout << "########################" << std::endl;
        std::cout << std::endl;

        if (!analysis_)
        {
            std::cout << "Attention! Analysys is not initialized for this solver." << std::endl;
            return;
        }

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
                std::cout << "  Error, convergence analysis stopped." << std::endl;
                return;
            }
        }

        std::cout << std::endl;
        std::cout << "Convergence rates: ";

        for (size_t i = 0; i < n_ - 1; ++i)
        {
            conv_rates_.push_back(std::log10(errors_[i + 1] / errors_[i]) / (std::log10(static_cast<double>(N_ref_[i]) / N_ref_[i + 1])));
            std::cout << conv_rates_.back() << " ";
        }
        std::cout << std::endl
                  << std::endl;
    }

    void exportConvergence(const std::string &fname = "convergence")
    {
        std::ofstream fsolution("solutions/" + fname + ".dat", std::ofstream::out);
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
};

#endif // __SCHEME_ANALYSIS__