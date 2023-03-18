#include <iostream>
#include <cmath>
#include <functional>
#include <array>
#include <vector>

#include "CrankNicolson.hpp"
#include "json.hpp"

using json = nlohmann::json;

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

int main(int argc, char **argv)
{

    // Inputs
    const function2 f(fun);
    const double y0 = 0;      // Initial condition
    const double T = 1;       // Final time
    const unsigned int N = 4; // Number of steps

    CrankNicolson solver(f, y0, T, N);
    if (solver.solve())
        solver.printSolution();
    else
        std::cout << "Error" << std::endl;

    // solver.computeOrder();

    // Convergence analysis

    const function1 u_ex(sol);
    const std::vector<unsigned int> N_ref({4, 8, 16, 32, 64, 128, 256});
    const std::function<double(std::vector<double>)> norm_max([](std::vector<double> error)
                                                              { return *std::max_element(error.begin(), error.end()); });

    CrankNicolson solver_analysis(f, y0, T, N, u_ex, N_ref, norm_max);
    solver_analysis.computeOrder();

    return 0;
}
