#pragma once

#include <vector>
#include <utility>

namespace sar {

// N-Queens: returns all solutions, each solution is a list of column positions for rows 0..n-1
std::vector<std::vector<int>> solveNQueens(int n);

// Subset Sum: returns all subsets summing up to target
std::vector<std::vector<int>> subsetSum(const std::vector<int>& set, int target);

// 0/1 Knapsack with Backtracking and bounding/pruning
std::pair<int, std::vector<int>> knapsackBacktracking(
    const std::vector<int>& weights,
    const std::vector<int>& values,
    int capacity);

} // namespace sar

