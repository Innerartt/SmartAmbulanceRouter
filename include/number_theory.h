#pragma once

#include <vector>

namespace sar {

// Extended Euclidean Algorithm: computes x, y such that a*x + b*y = gcd(a, b)
long long extendedGCD(long long a, long long b, long long& x, long long& y);

// Modular multiplicative inverse: a * x ≡ 1 (mod m)
long long modInverse(long long a, long long m);

// Chinese Remainder Theorem: solves system x ≡ rem[i] (mod mod[i])
// Assumes moduli are pairwise coprime. Returns solution x modulo prod(mod[i]).
long long chineseRemainderTheorem(
    const std::vector<long long>& remainders,
    const std::vector<long long>& moduli);

} // namespace sar

