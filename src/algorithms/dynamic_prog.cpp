/**
 * @file dynamic_prog.cpp
 * @brief Implementations of Dynamic Programming algorithms.
 */

#include "../../include/dynamic_prog.h"
#include <algorithm>
#include <numeric>
#include <sstream>

namespace sar {

// 0/1 Knapsack
std::pair<int, std::vector<int>> knapsack01(
    const std::vector<int>& weights,
    const std::vector<int>& values,
    int capacity) {
    int n = static_cast<int>(weights.size());
    if (n == 0 || capacity <= 0) return {0, {}};

    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));

    for (int i = 1; i <= n; ++i) {
        for (int w = 0; w <= capacity; ++w) {
            if (weights[i - 1] <= w) {
                dp[i][w] = std::max(dp[i - 1][w], dp[i - 1][w - weights[i - 1]] + values[i - 1]);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    // Backtrack to find chosen items
    std::vector<int> selected;
    int res = dp[n][capacity];
    int w = capacity;
    for (int i = n; i > 0 && res > 0; --i) {
        if (res != dp[i - 1][w]) {
            selected.push_back(i - 1);
            res -= values[i - 1];
            w -= weights[i - 1];
        }
    }
    std::reverse(selected.begin(), selected.end());
    return {dp[n][capacity], selected};
}

// Matrix Chain Multiplication
static void printOptimalParens(int i, int j, const std::vector<std::vector<int>>& s, std::stringstream& ss) {
    if (i == j) {
        ss << "A" << (i + 1);
    } else {
        ss << "(";
        printOptimalParens(i, s[i][j], s, ss);
        printOptimalParens(s[i][j] + 1, j, s, ss);
        ss << ")";
    }
}

std::pair<int, std::string> matrixChainOrder(const std::vector<int>& p) {
    int n = static_cast<int>(p.size()) - 1; // number of matrices
    if (n <= 0) return {0, ""};

    std::vector<std::vector<int>> m(n, std::vector<int>(n, 0));
    std::vector<std::vector<int>> s(n, std::vector<int>(n, 0));

    for (int len = 2; len <= n; ++len) {
        for (int i = 0; i <= n - len; ++i) {
            int j = i + len - 1;
            m[i][j] = 1e9;
            for (int k = i; k < j; ++k) {
                int q = m[i][k] + m[k + 1][j] + p[i] * p[k + 1] * p[j + 1];
                if (q < m[i][j]) {
                    m[i][j] = q;
                    s[i][j] = k;
                }
            }
        }
    }

    std::stringstream ss;
    printOptimalParens(0, n - 1, s, ss);
    return {m[0][n - 1], ss.str()};
}

// Longest Common Subsequence
std::string longestCommonSubsequence(const std::string& a, const std::string& b) {
    int m = static_cast<int>(a.size());
    int n = static_cast<int>(b.size());
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    // Reconstruct LCS string
    std::string lcs;
    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (a[i - 1] == b[j - 1]) {
            lcs.push_back(a[i - 1]);
            --i;
            --j;
        } else if (dp[i - 1][j] >= dp[i][j - 1]) {
            --i;
        } else {
            --j;
        }
    }
    std::reverse(lcs.begin(), lcs.end());
    return lcs;
}

// Optimal BST
double optimalBST(const std::vector<double>& keys, const std::vector<double>& freqs) {
    int n = static_cast<int>(keys.size());
    if (n == 0) return 0.0;

    std::vector<std::vector<double>> cost(n + 1, std::vector<double>(n + 1, 0.0));
    std::vector<std::vector<double>> weight(n + 1, std::vector<double>(n + 1, 0.0));

    for (int i = 0; i < n; ++i) {
        cost[i][i] = freqs[i];
        weight[i][i] = freqs[i];
    }

    for (int len = 2; len <= n; ++len) {
        for (int i = 0; i <= n - len; ++i) {
            int j = i + len - 1;
            cost[i][j] = 1e18;
            weight[i][j] = weight[i][j - 1] + freqs[j];

            for (int r = i; r <= j; ++r) {
                double c = ((r > i) ? cost[i][r - 1] : 0.0) +
                           ((r < j) ? cost[r + 1][j] : 0.0) +
                           weight[i][j];
                if (c < cost[i][j]) {
                    cost[i][j] = c;
                }
            }
        }
    }

    return cost[0][n - 1];
}

// Bellman-Ford
std::pair<std::unordered_map<int, double>, std::unordered_map<int, int>> bellmanFord(
    const Graph& g, int source, bool& hasNegativeCycle) {
    hasNegativeCycle = false;
    std::unordered_map<int, double> dist;
    std::unordered_map<int, int> parent;
    auto nodes = g.getNodes();

    for (int node : nodes) {
        dist[node] = INF;
        parent[node] = -1;
    }
    if (!g.hasNode(source)) return {dist, parent};
    dist[source] = 0.0;

    // Collect all directed edges
    std::vector<std::tuple<int, int, double>> edges;
    for (int u : nodes) {
        for (const auto& [v, w] : g.getNeighbors(u)) {
            edges.emplace_back(u, v, w);
        }
    }

    // Relax edges |V| - 1 times
    for (size_t i = 0; i + 1 < nodes.size(); ++i) {
        for (const auto& [u, v, w] : edges) {
            if (dist[u] < INF && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
            }
        }
    }

    // Check for negative weight cycles
    for (const auto& [u, v, w] : edges) {
        if (dist[u] < INF && dist[u] + w < dist[v]) {
            hasNegativeCycle = true;
            break;
        }
    }

    return {dist, parent};
}

// Floyd-Warshall
std::pair<std::vector<std::vector<double>>, std::vector<std::vector<int>>> floydWarshall(const Graph& g) {
    auto dist = g.getAdjacencyMatrix();
    size_t n = dist.size();
    std::vector<std::vector<int>> next(n, std::vector<int>(n, -1));

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (dist[i][j] < INF && i != j) {
                next[i][j] = static_cast<int>(j);
            }
        }
    }

    for (size_t k = 0; k < n; ++k) {
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                if (dist[i][k] < INF && dist[k][j] < INF) {
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        next[i][j] = next[i][k];
                    }
                }
            }
        }
    }

    return {dist, next};
}

// Held-Karp Bitmask DP for TSP
std::pair<double, std::vector<int>> tspDP(const std::vector<std::vector<double>>& dist) {
    int n = static_cast<int>(dist.size());
    if (n <= 1) return {0.0, {0}};
    if (n > 20) {
        // Guard against exponential explosion
        return {INF, {}};
    }

    int num_subsets = 1 << n;
    std::vector<std::vector<double>> memo(num_subsets, std::vector<double>(n, INF));
    std::vector<std::vector<int>> parent(num_subsets, std::vector<int>(n, -1));

    // Base case: starting from node 0
    memo[1][0] = 0.0;

    for (int mask = 1; mask < num_subsets; ++mask) {
        for (int u = 0; u < n; ++u) {
            if (!(mask & (1 << u))) continue;
            if (memo[mask][u] >= INF) continue;

            for (int v = 0; v < n; ++v) {
                if (mask & (1 << v)) continue;
                int next_mask = mask | (1 << v);
                double new_cost = memo[mask][u] + dist[u][v];
                if (new_cost < memo[next_mask][v]) {
                    memo[next_mask][v] = new_cost;
                    parent[next_mask][v] = u;
                }
            }
        }
    }

    // Close the tour by returning to node 0
    double min_cost = INF;
    int last_node = -1;
    int full_mask = num_subsets - 1;

    for (int u = 1; u < n; ++u) {
        if (memo[full_mask][u] < INF) {
            double total = memo[full_mask][u] + dist[u][0];
            if (total < min_cost) {
                min_cost = total;
                last_node = u;
            }
        }
    }

    if (last_node == -1) return {INF, {}};

    // Reconstruct tour
    std::vector<int> tour;
    int curr = last_node;
    int mask = full_mask;

    while (curr != -1) {
        tour.push_back(curr);
        int prev = parent[mask][curr];
        mask ^= (1 << curr);
        curr = prev;
    }
    std::reverse(tour.begin(), tour.end());
    tour.push_back(0); // return to start

    return {min_cost, tour};
}

} // namespace sar

