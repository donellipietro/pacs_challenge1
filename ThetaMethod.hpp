#ifndef __THETA_METHOD__

#define __THETA_METHOD__

#include <functional>
#include <array>
#include <vector>
#include <tuple>
#include <iomanip>
#include <fstream>

#include "basicZeroFun.hpp"
#include "SchemeAnalysis.hpp"
#include "Parameters.hpp"
#include "MuparserFun.hpp"

double finiteDiff(const function1 &f, const double x, const double h = 0.001);

class ThetaMethod : public SchemeAnalysis
{

protected:
  Parameters params_;
  function2 f_;
  double y0_;
  double T_;
  unsigned int N_;
  double h_;
  double theta_;

  std::vector<double> t_;
  std::vector<double> uh_;

public:
  ThetaMethod() { init(); }
  ThetaMethod(const std::string &filename) : params_(filename) { init(); }

  void createGrid()
  {
    for (std::size_t i = 0; i <= N_; ++i)
    {
      t_.push_back(0 + i * h_);
    }
  }

  void init()
  {
    std::cout << std::endl;
    std::cout << "########################" << std::endl;
    std::cout << "# Model initialization #" << std::endl;
    std::cout << "########################" << std::endl;
    std::cout << std::endl;

    if (!params_.sanityCheck())
      return;

    // Domain
    T_ = params_.domain.T;

    // Problem
    f_ = MuparserFun2(params_.problem.f);
    y0_ = params_.problem.y0;

    // Scheme
    N_ = params_.scheme.N;
    t_.reserve(N_ + 1);
    createGrid();
    uh_.reserve(N_ + 1);
    h_ = T_ / N_;
    theta_ = params_.scheme.theta;

    // Analysis
    analysis_ = params_.analysis.analysis;
    uex_ = MuparserFun1(params_.analysis.exact_solution);
    N_ref_ = params_.analysis.N_ref;
    norm_ = params_.analysis.norm;
    SchemeAnalysis::init();

    std::cout << "Initialization completed!" << std::endl;
  }

  bool solve()
  {

    // Initilaization of the scheme
    double un = y0_;
    double tn = t_.front();
    function1 F([this, &tn, &un](double x)
                { return x - theta_ * h_ * f_(tn + h_, x) - (1 - theta_) * h_ * f_(tn, un) - un; });

    // Initialization of Newton
    bool status_newton = true;
    function1 dF([&F](double x)
                 { return finiteDiff(F, x); });

    // Iterative algorithm
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
        uh_.push_back(un);
        tn = t_[i];
      }
    }

    return true;
  }

  std::array<std::vector<double>, 2> getResult() const { return {t_, uh_}; }
  const std::vector<double> &gett() const { return t_; }
  const std::vector<double> &getu() const { return uh_; }

  void printSolution()
  {
    std::cout << std::endl;
    std::cout << std::left << std::setw(8) << "t"
              << "\t"
              << "u" << std::endl;
    for (std::size_t i = 0; i <= N_; ++i)
    {
      std::cout << std::left << std::setw(8) << t_[i] << "\t" << uh_[i] << std::endl;
    }
  }

  void exportSolution(const std::string &fname = "solution")
  {
    std::ofstream fsolution("results/" + fname + ".dat", std::ofstream::out);
    for (std::size_t i = 0; i <= N_; ++i)
    {
      fsolution << t_[i] << "\t" << uh_[i];
      if (analysis_)
        fsolution << "\t" << uex_(t_[i]);
      fsolution << std::endl;
    }
    fsolution.close();
  }

  void setN(unsigned int N)
  {
    N_ = N;
    h_ = T_ / N_;

    t_.clear();
    uh_.clear();
    t_.reserve(N_ + 1);
    uh_.reserve(N_ + 1);

    createGrid();
  }

  void restoreN()
  {
    N_ = params_.scheme.N;
  }
};

/*

class CrankNicolson : public ThetaMethod
{
public:
  CrankNicolson(const function2 &f,
                const double y0,
                const double T,
                const unsigned int N,
                const double theta)
      : ThetaMethod{f, y0, T, N, 1 / 2} {}

  CrankNicolson(const function2 &f,
                const double y0,
                const double T,
                const unsigned int N,
                const double theta,
                const function1 &uh_ex,
                const std::vector<unsigned int> &N_ref,
                Norms norm)
      : ThetaMethod{f, y0, T, N, 1 / 2, uh_ex, N_ref, norm} {}
};

class BackwardEuler : public ThetaMethod
{
public:
  BackwardEuler(const function2 &f,
                const double y0,
                const double T,
                const unsigned int N,
                const double theta)
      : ThetaMethod{f, y0, T, N, 1 / 2} {}

  BackwardEuler(const function2 &f,
                const double y0,
                const double T,
                const unsigned int N,
                const double theta,
                const function1 &uh_ex,
                const std::vector<unsigned int> &N_ref,
                Norms norm)
      : ThetaMethod{f, y0, T, N, 1 / 2, uh_ex, N_ref, norm} {}
};

class ForwardEuler : public ThetaMethod
{
public:
  ForwardEuler(const function2 &f,
               const double y0,
               const double T,
               const unsigned int N,
               const double theta)
      : ThetaMethod{f, y0, T, N, 1 / 2} {}

  ForwardEuler(const function2 &f,
               const double y0,
               const double T,
               const unsigned int N,
               const double theta,
               const function1 &uh_ex,
               const std::vector<unsigned int> &N_ref,
               Norms norm)
      : ThetaMethod{f, y0, T, N, 1 / 2, uh_ex, N_ref, norm} {}
};

*/

double
finiteDiff(const function1 &f, const double x, const double h)
{
  return (f(x + h) - f(x - h)) / (2 * h);
}

#endif // __CRANK_NICOLSON__
