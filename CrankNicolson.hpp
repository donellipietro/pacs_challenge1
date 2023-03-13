#ifndef __CRANK_NICOLSON__

#define __CRANK_NICOLSON__

#include <functional>
#include <array>
#include <vector>
#include <tuple>
#include "basicZeroFun.hpp"

using function1 = std::function<double(double)>;
using function2 = std::function<double(double, double)>;

double
finiteDiff(const function1 &f, const double x, const double h = 0.001)
{
  return (f(x + h) - f(x - h)) / (2 * h);
}

class CrankNicolson
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
                const unsigned int N) : f_(f),
                                        y0_(y0),
                                        T_(T),
                                        N_(N),
                                        h_(T / N)

  {
    t_.reserve(N + 1);
    u_.reserve(N + 1);

    createGrid();
  }

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

  std::array<std::vector<double>, 2> getResult() const
  {
    std::array<std::vector<double>, 2> result({t_, u_});
    return result;
  }

  void printTime()
  {
    std::cout << t_.size() << std::endl;
    for (auto t : t_)
    {
      std::cout << t << std::endl;
    }
  }

  void printSolution()
  {
    std::cout << "t\tu" << std::endl;
    for (std::size_t i = 0; i <= N_; ++i)
    {
      std::cout << t_[i] << "\t" << u_[i] << std::endl;
    }
  }
};

#endif // __CRANK_NICOLSON__