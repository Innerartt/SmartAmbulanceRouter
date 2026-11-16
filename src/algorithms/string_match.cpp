/**
 * @file string_match.cpp
 * @brief Implementations of Naive, Rabin-Karp, Finite Automaton, and KMP string search.
 */

#include "../../include/string_match.h"
#include <vector>
#include <algorithm>

namespace sar {

// Naive String Search
std::vector<int> naiveStringMatch(const std::string& text, const std::string& pattern) {
    std::vector<int> matches;
    int n = static_cast<int>(text.size());
    int m = static_cast<int>(pattern.size());
    if (m == 0 || n < m) return matches;

    for (int i = 0; i <= n - m; ++i) {
        int j = 0;
        while (j < m && text[i + j] == pattern[j]) {
            ++j;
        }
        if (j == m) {
            matches.push_back(i);
        }
    }
    return matches;
}

// Rabin-Karp
std::vector<int> rabinKarp(const std::string& text, const std::string& pattern, int primeBase, int primeMod) {
    std::vector<int> matches;
    int n = static_cast<int>(text.size());
    int m = static_cast<int>(pattern.size());
    if (m == 0 || n < m) return matches;

    long long h = 1;
    for (int i = 0; i < m - 1; ++i) {
        h = (h * primeBase) % primeMod;
    }

    long long p_hash = 0;
    long long t_hash = 0;

    for (int i = 0; i < m; ++i) {
        p_hash = (primeBase * p_hash + static_cast<unsigned char>(pattern[i])) % primeMod;
        t_hash = (primeBase * t_hash + static_cast<unsigned char>(text[i])) % primeMod;
    }

    for (int i = 0; i <= n - m; ++i) {
        if (p_hash == t_hash) {
            // Check character by character
            bool match = true;
            for (int j = 0; j < m; ++j) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            if (match) matches.push_back(i);
        }

        if (i < n - m) {
            t_hash = (primeBase * (t_hash - static_cast<unsigned char>(text[i]) * h) +
                      static_cast<unsigned char>(text[i + m])) % primeMod;
            if (t_hash < 0) t_hash += primeMod;
        }
    }
    return matches;
}

// Finite Automaton Matcher
static const int ALPHABET_SIZE = 256;

static std::vector<std::vector<int>> buildTransitionTable(const std::string& pattern) {
    int m = static_cast<int>(pattern.size());
    std::vector<std::vector<int>> TF(m + 1, std::vector<int>(ALPHABET_SIZE, 0));

    for (int state = 0; state <= m; ++state) {
        for (int x = 0; x < ALPHABET_SIZE; ++x) {
            if (state < m && x == static_cast<unsigned char>(pattern[state])) {
                TF[state][x] = state + 1;
            } else {
                for (int next_state = state; next_state > 0; --next_state) {
                    if (pattern[next_state - 1] == static_cast<char>(x)) {
                        bool match = true;
                        for (int i = 0; i < next_state - 1; ++i) {
                            if (pattern[i] != pattern[state - next_state + 1 + i]) {
                                match = false;
                                break;
                            }
                        }
                        if (match) {
                            TF[state][x] = next_state;
                            break;
                        }
                    }
                }
            }
        }
    }
    return TF;
}

std::vector<int> faStringMatch(const std::string& text, const std::string& pattern) {
    std::vector<int> matches;
    int n = static_cast<int>(text.size());
    int m = static_cast<int>(pattern.size());
    if (m == 0 || n < m) return matches;

    auto TF = buildTransitionTable(pattern);
    int state = 0;

    for (int i = 0; i < n; ++i) {
        state = TF[state][static_cast<unsigned char>(text[i])];
        if (state == m) {
            matches.push_back(i - m + 1);
        }
    }
    return matches;
}

// Knuth-Morris-Pratt (KMP)
std::vector<int> computeKMPFailure(const std::string& pattern) {
    int m = static_cast<int>(pattern.size());
    std::vector<int> lps(m, 0);
    int len = 0;
    int i = 1;

    while (i < m) {
        if (pattern[i] == pattern[len]) {
            ++len;
            lps[i] = len;
            ++i;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                ++i;
            }
        }
    }
    return lps;
}

std::vector<int> kmpSearch(const std::string& text, const std::string& pattern) {
    std::vector<int> matches;
    int n = static_cast<int>(text.size());
    int m = static_cast<int>(pattern.size());
    if (m == 0 || n < m) return matches;

    auto lps = computeKMPFailure(pattern);
    int i = 0; // index in text
    int j = 0; // index in pattern

    while (i < n) {
        if (pattern[j] == text[i]) {
            ++i;
            ++j;
        }

        if (j == m) {
            matches.push_back(i - j);
            j = lps[j - 1];
        } else if (i < n && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                ++i;
            }
        }
    }
    return matches;
}

} // namespace sar

