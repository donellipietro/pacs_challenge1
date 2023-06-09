#ifndef __SCHEME_ANALYSIS__

#define __SCHEME_ANALYSIS__

#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <functional>
#include <iomanip>
#include <algorithm>

#include "Parameters.hpp"

double norm_Linf(const std::vector<double> &x);
double norm_L1(const std::vector<double> &x);
double norm_L2(const std::vector<double> &x);

class SchemeAnalysis
{
protected:
    bool analysis_;
    bool performing_analysis_ = false;
    function1 uex_;
    std::vector<unsigned int> N_ref_;
    unsigned int n_;
    std::vector<double> errors_;
    std::vector<double> conv_rates_;
    Norms norm_;
    bool plots_;

public:
    SchemeAnalysis() = default;

    void computeOrder();

    // Pure virtual method that the numeric scheme must expose
    // to interact with SchemeAnalysis
    virtual bool solve() = 0;

protected:
    void init();
    void exportConvergence() const;

    // Pure virtual methods that the numeric scheme must expose
    // to interact with SchemeAnalysis
    virtual void setN(const unsigned int N) = 0;
    virtual void restoreN() = 0;
    virtual const std::vector<double> &gett() const = 0;
    virtual const std::vector<double> &getu() const = 0;

private:
    double norm(std::vector<double> &x) const;
    double computeError() const;
};

#include "SchemeAnalysis.cpp"

#endif // __SCHEME_ANALYSIS__