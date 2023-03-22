#include "Parameters.hpp"

// Initializes the parameters from a json file
Parameters::Parameters(const std::string &fname)
{
    std::ifstream fdata(fname);

    if (!fdata)
    {
        errors.push_back("Error: Parameter file " + fname + " does not exist.");
        std::cerr << errors.back() << std::endl;
        fdata.close();
        return;
    }
    else
    {
        json data = json::parse(fdata);

        // Domain parameters
        domain.T = data["domain"].value("T", domain.T);

        // Problem parameters
        if (data["problem"]["f"].is_null() || data["problem"]["y0"].is_null())
        {
            errors.push_back("Bad initialization: In a custom initialization the focing term and the initial datum must be provided.");
            std::cerr << errors.back() << std::endl;
            fdata.close();
            return;
        }
        problem.y0 = data["problem"]["y0"].get<double>();
        problem.f = data["problem"]["f"].get<std::string>();

        // Scheme parameters
        scheme.N = data["scheme"].value("N", scheme.N);
        scheme.theta = data["scheme"].value("theta", scheme.theta);

        // Analysis parameters
        analysis.analysis = data.contains("analysis");
        if (analysis.analysis)
        {
            if (data["analysis"]["exact_solution"].is_null())
            {
                errors.push_back("Bad initialization: When the analysis is initialized the exact solution must be provided.");
                std::cerr << errors.back() << std::endl;
                fdata.close();
                return;
            }
            analysis.exact_solution = data["analysis"]["exact_solution"].get<std::string>();
            analysis.N_ref = data["analysis"].value("N_ref", analysis.N_ref);
            analysis.norm = static_cast<Norms>(data["analysis"].value("norm", analysis.norm));
            analysis.plots = data["analysis"].value("plots", analysis.plots);
        }
        else
        {
            analysis.exact_solution = "";
            analysis.N_ref = std::vector<unsigned int>{};
        }
    }

    fdata.close();
}

// Checks that no errors occurred during the initialization phase
bool Parameters::sanityCheck() const
{
    for (auto error : errors)
        std::cout << error << std::endl;
    return errors.size() == 0;
}

// Overloading of the operator<< so that model parameters can be printed easily
std::ostream &operator<<(std::ostream &os, const Parameters &params)
{

    os << std::endl;
    os << "##############" << std::endl;
    os << "# Parameters #" << std::endl;
    os << "##############" << std::endl;
    os << std::endl;

    if (!params.sanityCheck())
    {
        return os;
    }

    os << "Domain:" << std::endl
       << "- T: " << params.domain.T << std::endl
       << std::endl;

    os << "Problem:" << std::endl
       << "- Forcing term: " << params.problem.f << std::endl
       << "- Initial datum (y0): " << params.problem.y0 << std::endl
       << std::endl;

    os << "Scheme:" << std::endl
       << "- N: " << params.scheme.N << std::endl
       << "- Theta: " << params.scheme.theta << std::endl
       << std::endl;

    os << "Analysis: ";
    if (!params.analysis.analysis)
    {
        os << "not initialized" << std::endl
           << std::endl;
    }
    else
    {
        os << std::endl
           << "- Exact solution: " << params.analysis.exact_solution << std::endl
           << "- Refinements vector: { ";
        for (auto N : params.analysis.N_ref)
        {
            os << N << " ";
        }
        os << "}" << std::endl
           << "- Norm: " << static_cast<int>(params.analysis.norm) << std::endl
           << "- Plots: " << (params.analysis.plots ? "Yes" : "No") << std::endl
           << std::endl;
    }

    return os;
}