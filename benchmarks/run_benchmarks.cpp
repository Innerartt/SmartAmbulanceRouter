/**
 * @file run_benchmarks.cpp
 * @brief Benchmark suite comparing execution time across problem sizes.
 *
 * Author: Vivek Kumar Thakur (UPES)
 */

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include "../include/benchmark.h"
#include "../include/sorting.h"
#include "../include/divide_conquer.h"
#include "../include/greedy.h"
#include "../include/dynamic_prog.h"
#include "../include/backtracking.h"
#include "../include/branch_bound.h"
#include "../include/string_match.h"
#include "../include/graph.h"

using namespace sar;

void benchmarkSorting() {
    std::cout << "\n>>> BENCHMARK 1: SORTING (Merge Sort vs Quick Sort) <<<\n";
    std::vector<int> sizes = {1000, 10000, 50000};
    std::vector<BenchmarkResult> results;

    for (int n : sizes) {
        std::vector<int> original(n);
        for (int i = 0; i < n; ++i) original[i] = rand() % 1000000;

        auto a1 = original;
        results.push_back(runBenchmark("Merge Sort", "N = " + std::to_string(n), [&]() {
            mergeSort(a1);
        }, 5));

        auto a2 = original;
        results.push_back(runBenchmark("Quick Sort", "N = " + std::to_string(n), [&]() {
            quickSort(a2);
        }, 5));
    }
    printBenchmarkTable(results);
}

void benchmarkStringMatching() {
    std::cout << "\n>>> BENCHMARK 2: STRING MATCHING (Naive vs Rabin-Karp vs KMP) <<<\n";
    std::string text;
    for (int i = 0; i < 20000; ++i) {
        text += "ABCDEFGHIJKLM"[rand() % 13];
    }
    std::string pattern = "KLMABC";

    std::vector<BenchmarkResult> results;
    results.push_back(runBenchmark("Naive Matching", "20k Text, len 6", [&]() {
        naiveStringMatch(text, pattern);
    }, 10));

    results.push_back(runBenchmark("Rabin-Karp", "20k Text, len 6", [&]() {
        rabinKarp(text, pattern);
    }, 10));

    results.push_back(runBenchmark("KMP Matching", "20k Text, len 6", [&]() {
        kmpSearch(text, pattern);
    }, 10));

    printBenchmarkTable(results);
    compareBenchmarks(results);
}

void benchmarkKnapsack() {
    std::cout << "\n>>> BENCHMARK 3: 0/1 KNAPSACK (DP vs Backtracking vs Branch & Bound) <<<\n";
    int n = 18;
    int cap = 50;
    std::vector<int> weights(n);
    std::vector<int> values(n);
    for (int i = 0; i < n; ++i) {
        weights[i] = 1 + rand() % 15;
        values[i] = 5 + rand() % 30;
    }

    std::vector<BenchmarkResult> results;

    results.push_back(runBenchmark("Dynamic Programming", "N = 18 items", [&]() {
        knapsack01(weights, values, cap);
    }, 10));

    results.push_back(runBenchmark("Backtracking", "N = 18 items", [&]() {
        knapsackBacktracking(weights, values, cap);
    }, 10));

    results.push_back(runBenchmark("FIFO Branch & Bound", "N = 18 items", [&]() {
        knapsackFIFO_BB(weights, values, cap);
    }, 10));

    results.push_back(runBenchmark("Least-Cost B&B", "N = 18 items", [&]() {
        knapsackLC_BB(weights, values, cap);
    }, 10));

    printBenchmarkTable(results);
    compareBenchmarks(results);
}

int main() {
    std::cout << "====================================================================\n";
    std::cout << "  SMART AMBULANCE ROUTER — COMPREHENSIVE BENCHMARK RUNNER           \n";
    std::cout << "====================================================================\n";

    benchmarkSorting();
    benchmarkStringMatching();
    benchmarkKnapsack();

    std::cout << "All benchmarks completed successfully!\n";
    return 0;
}

