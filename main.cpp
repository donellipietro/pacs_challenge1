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
    ThetaMethod solver(filename);
    solver.solve();
    auto solution = solver.getResult();
    solver.computeOrder();

    return 0;
}
