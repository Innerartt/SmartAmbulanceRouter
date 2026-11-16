#pragma once

#include <vector>
#include <utility>

namespace sar {

struct BBNode {
    int level;
    int profit;
    int weight;
    double bound;
    std::vector<int> items;
};

// Branch and Bound variants for 0/1 Knapsack
std::pair<int, std::vector<int>> knapsackFIFO_BB(
    const std::vector<int>& weights,
    const std::vector<int>& values,
    int capacity);

std::pair<int, std::vector<int>> knapsackLIFO_BB(
    const std::vector<int>& weights,
    const std::vector<int>& values,
    int capacity);

std::pair<int, std::vector<int>> knapsackLC_BB(
    const std::vector<int>& weights,
    const std::vector<int>& values,
    int capacity);

// Branch and Bound for TSP
std::pair<double, std::vector<int>> tspBranchBound(
    const std::vector<std::vector<double>>& dist);

} // namespace sar

