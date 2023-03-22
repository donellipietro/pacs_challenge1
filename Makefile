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

.PHONY = all clean distclean
.DEFAULT_GOAL = all

all: $(EXEC)

# $(OBJS): $(SRCS) $(HEADERS)

main.o: main.cpp

main_test.o: main_test.cpp Parameters.hpp ThetaMethod.hpp SchemeAnalysis.hpp

main_test: main_test.o
# $(EXEC): $(OBJS)


clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) $(EXEC)
	$(RM) *~
	$(RM) results/*

run: $(EXEC) data/data.json
	./main
	sh plot.sh

test: main_test data/
	./main_test
