#include <iostream>
#include <cmath>
#include <functional>
#include <array>
#include <vector>
#include <fstream>
#include <string>

#include "CrankNicolson.hpp"
#include "json.hpp"
#include "MuparserFun.hpp"

using json = nlohmann::json;

using function1 = std::function<double(double)>;
using function2 = std::function<double(double, double)>;

int main(int argc, char **argv)
{

    // Solver parameters
    std::ifstream fdata("data.json");
    json data = json::parse(fdata);
    const function2 f(MuparserFun2(data["problem"]["f"].get<std::string>()));
    const double y0 = data["problem"].value("y0", 0.0);
    const double T = data["domain"].value("T", 1.0);
    const unsigned int N = data["numerical_scheme"].value("N", 4);

    // Solver
    CrankNicolson solver(f, y0, T, N);
    if (solver.solve())
        std::cout << "Solved" << std::endl; // solver.printSolution();
    else
        std::cout << "Error" << std::endl;

    solver.computeOrder();

    // Scheme Analysis parameters
    const function1 u_ex(MuparserFun1(data["scheme_analyis"]["sol"].get<std::string>()));
    const std::vector<unsigned int> N_ref = data["scheme_analyis"]["N_ref"].get<std::vector<unsigned int>>();
    const std::function<double(std::vector<double>)> norm_max([](std::vector<double> error)
                                                              { return *std::max_element(error.begin(), error.end()); });

    // Convergence order
    CrankNicolson solver_analysis(f, y0, T, N, u_ex, N_ref, norm_max);
    solver_analysis.computeOrder();

    return 0;
}
