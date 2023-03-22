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

  std::array<std::vector<double>, 2> getResult() const { return {t_, uh_}; }
  const std::vector<double> &gett() const { return t_; }
  const std::vector<double> &getu() const { return uh_; }

  void initScheme();
  void init();

  bool solve();

  void printSolution() const;
  void exportSolution(const std::string &fname) const;

  void setN(unsigned int N);
  void restoreN();
};

#include "ThetaMethod.cpp"

#endif // __THETA_METHOD__
