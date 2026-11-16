CXX ?= g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2
INCLUDES = -Iinclude

SRCS = src/core/domain.cpp \
       src/core/persistence.cpp \
       src/core/benchmark.cpp \
       src/core/dispatcher.cpp \
       src/algorithms/sorting.cpp \
       src/algorithms/search.cpp \
       src/algorithms/divide_conquer.cpp \
       src/algorithms/greedy.cpp \
       src/algorithms/dynamic_prog.cpp \
       src/algorithms/backtracking.cpp \
       src/algorithms/branch_bound.cpp \
       src/algorithms/flow.cpp \
       src/algorithms/number_theory.cpp \
       src/algorithms/string_match.cpp \
       src/ui/menu.cpp

MAIN_SRC = src/main.cpp
TEST_SRC = tests/test_runner.cpp
BENCH_SRC = benchmarks/run_benchmarks.cpp

all: smart_ambulance run_tests run_benchmarks

smart_ambulance: $(SRCS) $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

run_tests: $(SRCS) $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

run_benchmarks: $(SRCS) $(BENCH_SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

clean:
	rm -f smart_ambulance smart_ambulance.exe run_tests run_tests.exe run_benchmarks run_benchmarks.exe

.PHONY: all clean

