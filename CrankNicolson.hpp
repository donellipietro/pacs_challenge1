#ifndef __CRANK_NICOLSON__

#define __CRANK_NICOLSON__

#include <functional>
#include <array>
#include <vector>
#include <tuple>
#include <iomanip>
#include <fstream>

#include "basicZeroFun.hpp"
#include "SchemeAnalysis.hpp"

using function1 = std::function<double(double)>;
using function2 = std::function<double(double, double)>;

double
finiteDiff(const function1 &f, const double x, const double h = 0.001)
{
  return (f(x + h) - f(x - h)) / (2 * h);
}

class CrankNicolson : public SchemeAnalysis
{

private:
  function2 f_;
  double y0_;
  double T_;
  unsigned int N_;
  double h_;

  std::vector<double> t_;
  std::vector<double> u_;

public:
  CrankNicolson(const function2 &f,
                const double y0,
                const double T,
                const unsigned int N)
      : f_{f},
        y0_{y0},
        T_{T},
        N_{N},
        h_{T / N}
  {
    t_.reserve(N + 1);
    u_.reserve(N + 1);

    createGrid();
  }

  CrankNicolson(const function2 &f,
                const double y0,
                const double T,
                const unsigned int N,
                const function1 &u_ex,
                const std::vector<unsigned int> &N_ref,
                const std::function<double(std::vector<double>)> &norm)
      : SchemeAnalysis{u_ex, N_ref, norm},
        f_{f},
        y0_{y0},
        T_{T},
        N_{N},
        h_{T / N}
  {
    t_.reserve(N + 1);
    u_.reserve(N + 1);

    createGrid();
  }

  // Copy constructors?
  /*
    CrankNicolson(const CtrankNicolson & CN)
    CrankNicolson(const CtrankNicolson & CN,
                  const function1 &u_ex,
                  const std::vector<unsigned int> &N_ref,
                  const std::function<double(std::vector<double>)> &norm)
  */

  void createGrid()
  {
    for (std::size_t i = 0; i <= N_; ++i)
    {
      t_.push_back(0 + i * h_);
    }
  }

  bool solve()
  {

    // Initilaization of the scheme
    double un = y0_;
    double tn = t_.front();
    function1 F([this, &tn, &un](double x)
                { return x - h_ / 2 * (f_(tn + h_, x) + f_(tn + h_, un)) - un; });

    // Initialization of Newton
    bool status_newton = true;
    function1 dF([&F](double x)
                 { return finiteDiff(F, x); });

    // Iterative algoritmh
    for (std::size_t i = 0; i <= N_; ++i)
    {
      std::tie(un, status_newton) = apsc::Newton(F, dF, un);
      if (!status_newton)
      {
        std::cerr << "Cannot find solution" << std::endl;
        return false;
      }
      else
      {
        u_.push_back(un);
        tn = t_[i];
      }
    }

    return true;
  }

  std::array<std::vector<double>, 2> getResult() const { return {t_, u_}; }
  const std::vector<double> &gett() const { return t_; }
  const std::vector<double> &getu() const { return u_; }

  void printSolution()
  {
    std::cout << std::endl;
    std::cout << std::left << std::setw(8) << "t"
              << "\t"
              << "u" << std::endl;
    for (std::size_t i = 0; i <= N_; ++i)
    {
      std::cout << std::left << std::setw(8) << t_[i] << "\t" << u_[i] << std::endl;
    }
  }

  void exportSolution(const std::string &fname = "solution")
  {
    std::ofstream fsolution("solutions/" + fname + ".dat", std::ofstream::out);
    for (std::size_t i = 0; i <= N_; ++i)
    {
      fsolution << t_[i] << "\t" << u_[i];
      if (analysis_)
        fsolution << "\t" << u_ex_(t_[i]);
      fsolution << std::endl;
    }
    fsolution.close();
  }

  void setN(unsigned int N)
  {
    N_ = N;
    h_ = T_ / N_;

    t_.clear();
    u_.clear();
    t_.reserve(N_ + 1);
    u_.reserve(N_ + 1);

    createGrid();
  }
};

#endif // __CRANK_NICOLSON__
