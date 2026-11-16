/**
 * @file number_theory.cpp
 * @brief Implementations of Extended Euclidean Algorithm and Chinese Remainder Theorem.
 */

#include "../../include/number_theory.h"
#include <numeric>

namespace sar {

long long extendedGCD(long long a, long long b, long long& x, long long& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    long long x1 = 0, y1 = 0;
    long long gcd = extendedGCD(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return gcd;
}

long long modInverse(long long a, long long m) {
    long long x = 0, y = 0;
    long long g = extendedGCD(a, m, x, y);
    if (g != 1) return -1; // Inverse does not exist
    return (x % m + m) % m;
}

long long chineseRemainderTheorem(
    const std::vector<long long>& remainders,
    const std::vector<long long>& moduli) {
    size_t k = remainders.size();
    if (k == 0 || k != moduli.size()) return -1;

    long long prod = 1;
    for (size_t i = 0; i < k; ++i) {
        prod *= moduli[i];
    }

    long long result = 0;
    for (size_t i = 0; i < k; ++i) {
        long long pp = prod / moduli[i];
        long long inv = modInverse(pp, moduli[i]);
        if (inv == -1) return -1; // Not coprime
        result = (result + remainders[i] * pp % prod * inv) % prod;
    }

    return (result % prod + prod) % prod;
}

} // namespace sar

