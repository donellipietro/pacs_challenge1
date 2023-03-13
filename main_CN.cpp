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
        std::array<std::vector<double>, 2> result(solver.getResult());
        solver.printSolution();
    }
    else
        std::cout << "Error" << std::endl;

    return 0;
}
