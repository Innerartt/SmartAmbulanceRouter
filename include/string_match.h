#pragma once

#include <string>
#include <vector>

namespace sar {

// Naive String Search: O(n * m)
std::vector<int> naiveStringMatch(const std::string& text, const std::string& pattern);

// Rabin-Karp Search using rolling hash: O(n + m) avg
std::vector<int> rabinKarp(const std::string& text, const std::string& pattern, int primeBase = 256, int primeMod = 1000000007);

// Finite Automaton String Matcher: O(n + m*|Sigma|)
std::vector<int> faStringMatch(const std::string& text, const std::string& pattern);

// Knuth-Morris-Pratt (KMP) Search: O(n + m)
std::vector<int> computeKMPFailure(const std::string& pattern);
std::vector<int> kmpSearch(const std::string& text, const std::string& pattern);

} // namespace sar

