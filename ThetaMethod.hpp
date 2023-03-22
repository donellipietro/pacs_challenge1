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

  bool solve();
  void printSolution() const;

  Parameters parameters() const { return params_; };

protected:
  void initScheme();
  void init();
  const std::vector<double> &gett() const override { return t_; }
  const std::vector<double> &getu() const override { return uh_; }
  void setN(unsigned int N) override;
  void restoreN() override;
  void exportSolution() const;
};

#include "ThetaMethod.cpp"

#endif // __THETA_METHOD__
