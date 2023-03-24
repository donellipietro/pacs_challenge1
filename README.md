# Challenge 1 - Build a simple solver for a Cauchy problem

This code was developed for the course of **Advanced Programming for Scientific Computing** for the MSc. in Mathematical Engineering at Politecnico di Milano, A.Y. 2022/2023.

## Problem

It is of interest to implement a program that numerically solves a cauchy problem defined by a forcing term, an initial datum, and the domain of interest by exploiting theta-method.

## Description

```
.
|-- README.md
|-- Makefile
|-- MuparserFun.hpp
|-- Parameters.cpp
|-- Parameters.hpp
|-- SchemeAnalysis.cpp
|-- SchemeAnalysis.hpp
|-- ThetaMethod.cpp
|-- ThetaMethod.hpp
|-- main.cpp
|-- main_test.cpp
|-- plot_convergence.sh
|-- plot_solution.sh
|-- results
|-- data
|   |-- bad_initialization1.json
|   |-- bad_initialization2.json
|   |-- data.json
|   `-- no_analysis.json
|-- include
`-- lib
```

- [`Parameters.hpp`](Parameters.hpp) The class handles storing model parameters and reading them from json files making use of the ['JSON for Modern C++ library'](https://github.com/nlohmann/json). In particular, functional parameters (forcing term and exact solution) are parsed thanks to the ['muParser'](https://github.com/beltoforion/muparser) library. Methods are implemented in [`Parameters.cpp`](Parameters.cpp).

- [`SchemeAnalysis.hpp`](SchemeAnalysis.hpp) This is an interface that allows the computation of the order of convergence of a numerical method. The choice of the norm used to compute the errors is made through the parameters of the model to which it is applied; the norms implemented are: norm Linf, L1, L2.
In order to use this class, a numeric method must expose the following methods: 
  - `virtual void setN(const unsigned int N) = 0;`
  - `virtual void restoreN() = 0;`
  - `virtual const std::vector<double> &gett() const = 0;`
  - `virtual const std::vector<double> &getu() const = 0;`

Methods are implemented in [`SchemeAnalysis.cpp`](SchemeAnalysis.cpp).

- [`ThetaMethod.hpp`](ThetaMethod.hpp) This class implements the initialization of the numerical scheme based on the chosen parameters and the problem solver using theta-method, where theta is a parameter itself. As a default, the class is initialized with parameters related to the challenge. However, a constructor is also defined fot the initialization of the model using parameters contained within a `.json` file located in the `data` directory. In this case the file name must be specified as a constructor parameter. Utilities for exporting and displaying the results are also implemented along with the methods needed to use the numerical scheme analysis interface. The implementation of the methods is contained in [`ThetaMethod.cpp`](ThetaMethod.cpp).

- [`plot_solution.sh`](plot_solution.sh) and [`plot_convergence.sh`](plot_convergence.sh) open a gnuplot terminal and plot the solution and convergence based on the data exported during program execution. This data are exported only in the case where the parameter `plot` is set to `true`. In particular for convergence data, it is also necessary that the analysis is well defined by providing the exact solution of the problem of interest.

- [`main.cpp`](main.hpp) solves the problem of interest using the parameters specified in the `data/data.json` file. It is possible to specify a different parameters file thanks to the `-p` flag followed by the desired parameter file name (e.g. `main -p no_analysis.json`).

- [`main_test.cpp`](main_test.hpp) performs tests on the implemented features.

## How to use it

The `Makefile` file implements all the commands needed to compile and use the code. In particular, the following targets are available:
- `make` compiles the code and generates the executables.
- `make run` compiles the code and launch the main. If plots are requested in the parameter file of the problem of interest they are generated in the `results` directory. Moreover, the `p` argument is available to specify the name of the parameter file to be used (e.g. `make run p="no_analysis.json"`).
- `make test` compiles the code and launches the test.

## Author

Pietro Donelli ([@donellipietro](https://github.com/donellipietro))
