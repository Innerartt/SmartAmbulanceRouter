#pragma once

#include <vector>
#include <tuple>

namespace sar {

struct SubarrayResult {
    int low;
    int high;
    long long sum;
};

// Divide-and-Conquer Maximum Subarray
SubarrayResult maxSubarray(const std::vector<int>& arr);

// Modular fast exponentiation: (base^exp) % mod
long long fastPower(long long base, long long exp, long long mod);

// Celebrity identification using elimination: O(n)
// knows(i, j) is true if person i knows person j
int findCelebrity(const std::vector<std::vector<bool>>& knowsMatrix, int n);

// Classical Matrix Multiplication: O(n^3)
std::vector<std::vector<double>> matMulClassical(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B);

// Strassen's Matrix Multiplication: O(n^2.81)
std::vector<std::vector<double>> matMulStrassen(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B);

} // namespace sar

