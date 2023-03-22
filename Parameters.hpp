#ifndef __PARAMETERS__
#define __PARAMETERS__

#include <string>
#include <vector>
#include "json.hpp"
#include <iostream>
#include <fstream>

using json = nlohmann::json;

using function1 = std::function<double(double)>;
using function2 = std::function<double(double, double)>;

enum class Norms
{
    Linf,
    L1,
    L2
};

struct Domain
{
    // Domain is [0, T].
    double T = 1.;
};

struct Problem
{
    // Initial datum
    double y0 = 0.;

    // Forcing term
    std::string f = "-t * exp(-y)";
};

struct Scheme
{
    // Number of intervals (Number of nodes = N+1)
    unsigned int N = 8;

    // Theta method parameter (\theta \in [0,1])
    // - \theta = 0.0 : Forward Euler
    // - \theta = 0.5 : Crank Nicolson (default)
    // - \theta = 1.0 : Backward Euler
    double theta = 0.5;
};

struct Analysis
{
    // True if the analysis is initialized
    bool analysis = true;

    // Exact solution
    std::string exact_solution{"log(-t*t/2 + 1)"};

    // Refinements vector
    std::vector<unsigned int> N_ref{8, 16, 32, 64};

    // Norm used to compute the errors
    // - 0: Linf norm
    // - 1: L1 norm
    // - 2: L2 norm
    Norms norm = Norms::Linf;

    // If true plots are generated
    bool plots = true;
};

struct Parameters
{
    Parameters() = default;
    Parameters(const std::string &fname);
    bool sanityCheck() const;
    friend std::ostream &operator<<(std::ostream &os, const Parameters &params);

    Domain domain;
    Problem problem;
    Scheme scheme;
    Analysis analysis;

    std::vector<std::string> errors;
};

#include "Parameters.cpp"

#endif // __PARAMETERS__
