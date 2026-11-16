/**
 * @file backtracking.cpp
 * @brief Implementations of Backtracking algorithms.
 */

#include "../../include/backtracking.h"
#include <algorithm>
#include <cmath>

namespace sar {

// N-Queens
static bool isSafeQueen(int row, int col, const std::vector<int>& queens) {
    for (int r = 0; r < row; ++r) {
        int c = queens[r];
        if (c == col || std::abs(c - col) == std::abs(r - row)) {
            return false;
        }
    }
    return true;
}

static void nQueensHelper(int row, int n, std::vector<int>& queens,
                          std::vector<std::vector<int>>& solutions) {
    if (row == n) {
        solutions.push_back(queens);
        return;
    }
    for (int col = 0; col < n; ++col) {
        if (isSafeQueen(row, col, queens)) {
            queens[row] = col;
            nQueensHelper(row + 1, n, queens, solutions);
        }
    }
}

std::vector<std::vector<int>> solveNQueens(int n) {
    std::vector<std::vector<int>> solutions;
    if (n <= 0) return solutions;
    std::vector<int> queens(n, 0);
    nQueensHelper(0, n, queens, solutions);
    return solutions;
}

// Subset Sum
static void subsetSumHelper(const std::vector<int>& set, int index, int current_sum,
                            int target, std::vector<int>& current_subset,
                            std::vector<std::vector<int>>& result) {
    if (current_sum == target) {
        result.push_back(current_subset);
        return;
    }
    if (index >= static_cast<int>(set.size()) || current_sum > target) {
        return;
    }

    // Include set[index]
    current_subset.push_back(set[index]);
    subsetSumHelper(set, index + 1, current_sum + set[index], target, current_subset, result);

    // Backtrack (exclude set[index])
    current_subset.pop_back();
    subsetSumHelper(set, index + 1, current_sum, target, current_subset, result);
}

std::vector<std::vector<int>> subsetSum(const std::vector<int>& set, int target) {
    std::vector<std::vector<int>> result;
    std::vector<int> current;
    subsetSumHelper(set, 0, 0, target, current, result);
    return result;
}

// 0/1 Knapsack Backtracking
static void knapsackBTHelper(int index, int current_weight, int current_value,
                             int capacity, const std::vector<int>& w,
                             const std::vector<int>& v, std::vector<int>& current_items,
                             int& max_val, std::vector<int>& best_items) {
    int n = static_cast<int>(w.size());
    if (index == n) {
        if (current_value > max_val) {
            max_val = current_value;
            best_items = current_items;
        }
        return;
    }

    // Try including index if capacity permits
    if (current_weight + w[index] <= capacity) {
        current_items.push_back(index);
        knapsackBTHelper(index + 1, current_weight + w[index], current_value + v[index],
                         capacity, w, v, current_items, max_val, best_items);
        current_items.pop_back();
    }

    // Try excluding index
    knapsackBTHelper(index + 1, current_weight, current_value,
                     capacity, w, v, current_items, max_val, best_items);
}

std::pair<int, std::vector<int>> knapsackBacktracking(
    const std::vector<int>& weights,
    const std::vector<int>& values,
    int capacity) {
    int max_val = 0;
    std::vector<int> best_items;
    std::vector<int> current_items;
    knapsackBTHelper(0, 0, 0, capacity, weights, values, current_items, max_val, best_items);
    return {max_val, best_items};
}

} // namespace sar

