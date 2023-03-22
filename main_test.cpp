#include "Parameters.hpp"
#include "ThetaMethod.hpp"

int main(int argc, char **argv)
{
    /********************/
    /* Test: Parameters */
    /********************/

    /*

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

    */

    /**********************/
    /* Test: Constructors */
    /**********************/

    /*

    // Initialization solver no analysis
    ThetaMethod solver1("data/no_analysis.json");
    std::cout << solver1.parameters();
    solver1.solve();
    solver1.computeOrder();
    solver1.getResult();

    // Error in params
    ThetaMethod solver2("data/bad_initialization2.json"); // data/dat.json data/bad_initialization1.json
    std::cout << solver2.parameters();
    solver2.solve();
    solver2.computeOrder();

    // Default initialization
    ThetaMethod solver3;
    std::cout << solver3.parameters();
    solver3.solve();
    solver3.computeOrder();
    solver3.getResult();

    */

    /***************/
    /* Test: Norms */
    /***************/

    std::cout << std::endl;
    // std::vector<double> v{0, -0.2, -0.3, -0.4};
    std::vector<double> v{0, -0.00784318, -0.0161237, -0.0255397, -0.0370155, -0.0519525, -0.0727206, -0.103775, -0.154593};
    std::cout << "v = { ";
    for (auto vi : v)
        std::cout << vi << " ";
    std::cout << "}" << std::endl;
    std::cout << "- ||v||Linf = " << norm_Linf(v) << std::endl;
    std::cout << "- ||v||L1 = " << norm_L1(v) << std::endl;
    std::cout << "- ||v||L2 = " << norm_L2(v) << std::endl;

    return 0;
}
