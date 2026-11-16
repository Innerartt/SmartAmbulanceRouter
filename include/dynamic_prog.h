#pragma once

#include <vector>
#include <string>
#include <utility>
#include <unordered_map>
#include "graph.h"

namespace sar {

// 0/1 Knapsack: returns (max_value, list of selected item indices)
std::pair<int, std::vector<int>> knapsack01(
    const std::vector<int>& weights,
    const std::vector<int>& values,
    int capacity);

// Matrix Chain Multiplication: returns (min_multiplications, optimal_parens_string)
std::pair<int, std::string> matrixChainOrder(const std::vector<int>& dims);

// Longest Common Subsequence: returns the actual LCS string
std::string longestCommonSubsequence(const std::string& a, const std::string& b);

// Optimal Binary Search Tree: returns minimum expected search cost
double optimalBST(const std::vector<double>& keys, const std::vector<double>& freqs);

// Bellman-Ford Shortest Paths: detects negative weight cycles
// Returns (distances, parents), or empty if negative cycle detected
std::pair<std::unordered_map<int, double>, std::unordered_map<int, int>> bellmanFord(
    const Graph& g, int source, bool& hasNegativeCycle);

// Floyd-Warshall All-Pairs Shortest Path
// Returns (distance_matrix, next_hop_matrix)
std::pair<std::vector<std::vector<double>>, std::vector<std::vector<int>>> floydWarshall(const Graph& g);

// Bounded TSP via Held-Karp dynamic programming (bitmask)
// Suitable for N <= 20. Returns (min_tour_cost, tour_node_indices)
std::pair<double, std::vector<int>> tspDP(const std::vector<std::vector<double>>& dist);

} // namespace sar

