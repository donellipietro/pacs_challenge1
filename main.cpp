#include <iostream>
#include <cmath>
#include <functional>
#include <array>
#include <vector>

#include "CrankNicolson.hpp"

using function1 = std::function<double(double)>;
using function2 = std::function<double(double, double)>;

double fun(double t, double y)
{
    return -t * std::exp(-y);
}

double sol(double t)
{
    return std::log(-t * t / 2 + 1);
}

double computeError(const std::vector<double> &t, const std::vector<double> &u, const function1 &u_ex)
{
    double error = 0.;

    for (std::size_t i = 0; i < t.size(); ++i)
    {
        error = std::max(std::abs(u_ex(t[i]) - u[i]), error);
    }

    return error;
}

double computeError(const std::array<std::vector<double>, 2> &result, const function1 &u_ex)
{
    return computeError(result[0], result[1], u_ex);
}

int main(int argc, char **argv)
{

    // Inputs
    const function2 f(fun);
    const double y0 = 0;        // Initial condition
    const double T = 1;         // Final time
    const unsigned int N = 100; // Number of steps

    CrankNicolson solver(f, y0, T, N);
    if (solver.solve())
    {
        // solver.printSolution();
    }
    else
        std::cout << "Error" << std::endl;

    // Convergence analysis

    function1 u_ex(sol);
    const std::vector<unsigned int> N_ref({4, 8, 16, 32, 64, 128, 256});

    std::vector<double> errors;
    errors.reserve(N_ref.size());
    for (unsigned int N_conv : N_ref)
    {
        solver.setN(N_conv);
        solver.solve();
        // solver.printSolution();
        errors.push_back(computeError(solver.getResult(), u_ex));
    }

    std::cout << "Errors: ";
    for (size_t i = 0; i < N_ref.size(); ++i)
    {
        std::cout << errors[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Convergence Rates: ";
    for (size_t i = 0; i < N_ref.size() - 1; ++i)
    {
        std::cout << (std::log10(errors[i + 1] / errors[i]) / (std::log10(static_cast<double>(N_ref[i]) / N_ref[i + 1]))) << " ";
    }
    std::cout << std::endl;

    return 0;
}
