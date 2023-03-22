#include <iostream>

#include <array>
#include <vector>
#include <string>

#include "GetPot"
#include "ThetaMethod.hpp"

void printHelp()
{
    std::cout
        << "USAGE: main [-h] -p parameterFile (default: data.json)"
        << std::endl;
    std::cout << "-h --help this help" << std::endl;
}

int main(int argc, char **argv)
{

    // Help
    GetPot cl(argc, argv);
    if (cl.search(2, "-h", "--help"))
    {
        printHelp();
        return 0;
    }

    // Get file with parameter values
    std::string filename = "data/" + static_cast<std::string>(cl.follow("data.json", "-p"));

    // Solver
    // solver(filename);
    // ThetaMethod solver(filename);

    /*
    if (solver.solve())
    {
        std::cout << "Solved!" << std::endl;
        solver.printSolution();
        solver.exportSolution();
    }
    else
        std::cout << "Error!" << std::endl;

    solver.computeOrder();


    // Convergence order
    ThetaMethod solver_analysis(f, y0, T, N, theta, u_ex, N_ref, Norms::Linf);
    solver_analysis.solve();
    solver_analysis.exportSolution("solution");
    solver_analysis.computeOrder();
    solver_analysis.exportConvergence("convergence");
    */

    return 0;
}
