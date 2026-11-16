/**
 * @file divide_conquer.cpp
 * @brief Implementations of Divide & Conquer algorithms.
 */

#include "../../include/divide_conquer.h"
#include <climits>
#include <algorithm>
#include <stdexcept>

namespace sar {

// Helper for max crossing subarray
static SubarrayResult findMaxCrossingSubarray(const std::vector<int>& arr, int low, int mid, int high) {
    long long left_sum = LLONG_MIN;
    long long sum = 0;
    int max_left = mid;
    for (int i = mid; i >= low; --i) {
        sum += arr[i];
        if (sum > left_sum) {
            left_sum = sum;
            max_left = i;
        }
    }

    long long right_sum = LLONG_MIN;
    sum = 0;
    int max_right = mid + 1;
    for (int j = mid + 1; j <= high; ++j) {
        sum += arr[j];
        if (sum > right_sum) {
            right_sum = sum;
            max_right = j;
        }
    }

    return {max_left, max_right, left_sum + right_sum};
}

static SubarrayResult maxSubarrayRecursive(const std::vector<int>& arr, int low, int high) {
    if (low == high) {
        return {low, high, static_cast<long long>(arr[low])};
    }
    int mid = low + (high - low) / 2;
    SubarrayResult left_res = maxSubarrayRecursive(arr, low, mid);
    SubarrayResult right_res = maxSubarrayRecursive(arr, mid + 1, high);
    SubarrayResult cross_res = findMaxCrossingSubarray(arr, low, mid, high);

    if (left_res.sum >= right_res.sum && left_res.sum >= cross_res.sum) {
        return left_res;
    } else if (right_res.sum >= left_res.sum && right_res.sum >= cross_res.sum) {
        return right_res;
    } else {
        return cross_res;
    }
}

SubarrayResult maxSubarray(const std::vector<int>& arr) {
    if (arr.empty()) return {-1, -1, 0};
    return maxSubarrayRecursive(arr, 0, static_cast<int>(arr.size()) - 1);
}

long long fastPower(long long base, long long exp, long long mod) {
    if (mod <= 1) return 0;
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

int findCelebrity(const std::vector<std::vector<bool>>& knowsMatrix, int n) {
    if (n <= 0) return -1;
    int candidate = 0;
    // Step 1: Find potential celebrity in O(n)
    for (int i = 1; i < n; ++i) {
        if (knowsMatrix[candidate][i]) {
            // candidate knows i, so candidate cannot be celebrity
            candidate = i;
        }
    }

    // Step 2: Verify candidate
    for (int i = 0; i < n; ++i) {
        if (i != candidate) {
            // Candidate should not know i, and i must know candidate
            if (knowsMatrix[candidate][i] || !knowsMatrix[i][candidate]) {
                return -1;
            }
        }
    }
    return candidate;
}

std::vector<std::vector<double>> matMulClassical(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B) {
    size_t n = A.size();
    if (n == 0 || B.empty() || A[0].size() != B.size()) return {};
    size_t m = A[0].size();
    size_t p = B[0].size();

    std::vector<std::vector<double>> C(n, std::vector<double>(p, 0.0));
    for (size_t i = 0; i < n; ++i) {
        for (size_t k = 0; k < m; ++k) {
            double aik = A[i][k];
            for (size_t j = 0; j < p; ++j) {
                C[i][j] += aik * B[k][j];
            }
        }
    }
    return C;
}

// Matrix helper routines for Strassen
static std::vector<std::vector<double>> addMat(const std::vector<std::vector<double>>& A,
                                               const std::vector<std::vector<double>>& B) {
    size_t n = A.size();
    std::vector<std::vector<double>> C(n, std::vector<double>(n));
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < n; ++j)
            C[i][j] = A[i][j] + B[i][j];
    return C;
}

static std::vector<std::vector<double>> subMat(const std::vector<std::vector<double>>& A,
                                               const std::vector<std::vector<double>>& B) {
    size_t n = A.size();
    std::vector<std::vector<double>> C(n, std::vector<double>(n));
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < n; ++j)
            C[i][j] = A[i][j] - B[i][j];
    return C;
}

static std::vector<std::vector<double>> strassenRecursive(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B) {
    size_t n = A.size();
    if (n <= 2) {
        return matMulClassical(A, B);
    }

    size_t k = n / 2;
    std::vector<std::vector<double>> A11(k, std::vector<double>(k)),
                                    A12(k, std::vector<double>(k)),
                                    A21(k, std::vector<double>(k)),
                                    A22(k, std::vector<double>(k)),
                                    B11(k, std::vector<double>(k)),
                                    B12(k, std::vector<double>(k)),
                                    B21(k, std::vector<double>(k)),
                                    B22(k, std::vector<double>(k));

    for (size_t i = 0; i < k; ++i) {
        for (size_t j = 0; j < k; ++j) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + k];
            A21[i][j] = A[i + k][j];
            A22[i][j] = A[i + k][j + k];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + k];
            B21[i][j] = B[i + k][j];
            B22[i][j] = B[i + k][j + k];
        }
    }

    // 7 products
    auto M1 = strassenRecursive(addMat(A11, A22), addMat(B11, B22));
    auto M2 = strassenRecursive(addMat(A21, A22), B11);
    auto M3 = strassenRecursive(A11, subMat(B12, B22));
    auto M4 = strassenRecursive(A22, subMat(B21, B11));
    auto M5 = strassenRecursive(addMat(A11, A12), B22);
    auto M6 = strassenRecursive(subMat(A21, A11), addMat(B11, B12));
    auto M7 = strassenRecursive(subMat(A12, A22), addMat(B21, B22));

    auto C11 = addMat(subMat(addMat(M1, M4), M5), M7);
    auto C12 = addMat(M3, M5);
    auto C21 = addMat(M2, M4);
    auto C22 = addMat(addMat(subMat(M1, M2), M3), M6);

    std::vector<std::vector<double>> C(n, std::vector<double>(n));
    for (size_t i = 0; i < k; ++i) {
        for (size_t j = 0; j < k; ++j) {
            C[i][j] = C11[i][j];
            C[i][j + k] = C12[i][j];
            C[i + k][j] = C21[i][j];
            C[i + k][j + k] = C22[i][j];
        }
    }
    return C;
}

std::vector<std::vector<double>> matMulStrassen(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B) {
    size_t n = A.size();
    if (n == 0 || A[0].size() != n || B.size() != n || B[0].size() != n) {
        return matMulClassical(A, B);
    }
    // Pad to power of 2 if needed
    size_t m = 1;
    while (m < n) m <<= 1;
    if (m == n) {
        return strassenRecursive(A, B);
    }

    std::vector<std::vector<double>> APad(m, std::vector<double>(m, 0.0));
    std::vector<std::vector<double>> BPad(m, std::vector<double>(m, 0.0));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            APad[i][j] = A[i][j];
            BPad[i][j] = B[i][j];
        }
    }

    auto CPad = strassenRecursive(APad, BPad);
    std::vector<std::vector<double>> C(n, std::vector<double>(n));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            C[i][j] = CPad[i][j];
        }
    }
    return C;
}

} // namespace sar

