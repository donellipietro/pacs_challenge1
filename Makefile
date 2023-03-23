CXX ?= g++
CXXFLAGS ?= -std=c++20
CPPFLAGS ?= -O3 -Wall -I. -Wno-conversion-null -Wno-deprecated-declarations

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
HEADERS = $(wildcard *.hpp)

EXEC_sources = $(filter main%.cpp,$(SRCS))
EXEC = $(EXEC_sources:.cpp=)

CPPFLAGS += -I ./include # $(MY_PACS_ROOT)
LDFLAGS +=  -L ./lib -Wl,-rpath=./lib # $(MY_PACS_ROOT)
LDLIBS += -l muparser -l stdc++ -l m

.PHONY = all clean distclean run test
.DEFAULT_GOAL = all

all: $(EXEC)

# $(OBJS): $(SRCS) $(HEADERS)

main.o: main.cpp Parameters.hpp Parameters.cpp \
 SchemeAnalysis.hpp SchemeAnalysis.cpp \
 ThetaMethod.hpp ThetaMethod.cpp \
 MuParserFun.hpp

main_test.o: main_test.cpp Parameters.hpp Parameters.cpp \
 SchemeAnalysis.hpp SchemeAnalysis.cpp \
 ThetaMethod.hpp ThetaMethod.cpp \
 MuParserFun.hpp

# $(EXEC): $(OBJS)

main_test: main_test.o

main: main.o

run: main data/
	$(RM) results/*
	./main -p $(p)
	@ if [ -e results/solution.dat ]; then sh plot_solution.sh; fi
	@ if [ -e results/convergence.dat ]; then sh plot_convergence.sh; fi

test: main_test data/
	./main_test

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) $(EXEC)
	$(RM) *~
	$(RM) results/*
