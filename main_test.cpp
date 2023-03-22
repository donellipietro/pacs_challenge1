#include "Parameters.hpp"
#include "ThetaMethod.hpp"

int main(int argc, char **argv)
{
    /********************/
    /* Test: Parameters */
    /********************/

    // Default initialization
    Parameters params1;
    std::cout << params1;

    // File does not exist
    Parameters params2("data/dat.json");
    std::cout << params2;

    // Standard data
    Parameters params3("data/data.json");
    std::cout << params3;

    // Bad initialization: f not provided
    Parameters params4("data/bad_initialization1.json");
    std::cout << params4;

    // Bad initialization: exact solution not provided
    Parameters params5("data/bad_initialization2.json");
    std::cout << params5;

    // No analysis
    Parameters params6("data/no_analysis.json");
    std::cout << params6;

    /**********************/
    /* Test: Constructors */
    /**********************/

    // Default initialization
    ThetaMethod solver1;

    // Error in params
    ThetaMethod solver2("data/dat.json");

    // Initialization from parameter file
    ThetaMethod solver3("data/data.json");

    /***************/
    /* Test: Norms */
    /***************/

    std::cout << std::endl;
    std::vector<double> v{0, 1, -1, 2, -3};
    std::cout << "v = { ";
    for (auto vi : v)
        std::cout << vi << " ";
    std::cout << "}" << std::endl;
    std::cout << "- ||v||Linf = " << norm_Linf(v) << std::endl;
    std::cout << "- ||v||L1 = " << norm_L1(v) << std::endl;
    std::cout << "- ||v||L2 = " << norm_L2(v) << std::endl;

    return 0;
}
