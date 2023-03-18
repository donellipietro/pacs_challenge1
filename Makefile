CXX ?= g++
CXXFLAGS ?= -std=c++20
CPPFLAGS ?= -O3 -Wall -I. -Wno-conversion-null -Wno-deprecated-declarations

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
HEADERS = $(wildcard *.hpp)

EXEC_sources = $(filter main%.cpp,$(SRCS))
EXEC = $(EXEC_sources:.cpp=)

CPPFLAGS += -I $(MY_PACS_ROOT)/include
LDFLAGS +=  -L $(MY_PACS_ROOT)/lib -Wl,-rpath=$(MY_PACS_ROOT)/lib
LDLIBS += -l pacs -l muparser -l stdc++ -l m

.PHONY = all clean distclean
.DEFAULT_GOAL = all

all: $(EXEC)

$(OBJS): $(SRCS) $(HEADERS)

$(EXEC): $(OBJS)

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) $(EXEC)
	$(RM) *~

run: $(EXEC)
	./$(EXEC)
