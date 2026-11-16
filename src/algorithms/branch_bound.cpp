/**
 * @file branch_bound.cpp
 * @brief Implementations of FIFO, LIFO, and Least-Cost Branch and Bound algorithms.
 */

#include "../../include/branch_bound.h"
#include <queue>
#include <stack>
#include <algorithm>
#include <numeric>

namespace sar {

// Item helper for sorting items by ratio (value / weight)
struct KnapsackItem {
    int id;
    int weight;
    int value;
    double ratio;
};

// Calculate fractional knapsack upper bound for a node
static double calculateBound(const BBNode& u, int n, int capacity,
                             const std::vector<KnapsackItem>& items) {
    if (u.weight >= capacity) return 0.0;

    double profit_bound = u.profit;
    int j = u.level + 1;
    int total_weight = u.weight;

    while (j < n && total_weight + items[j].weight <= capacity) {
        total_weight += items[j].weight;
        profit_bound += items[j].value;
        ++j;
    }

    if (j < n) {
        profit_bound += (capacity - total_weight) * items[j].ratio;
    }

    return profit_bound;
}

// Common setup for Knapsack Branch and Bound
static std::vector<KnapsackItem> prepareItems(const std::vector<int>& weights,
                                              const std::vector<int>& values) {
    int n = static_cast<int>(weights.size());
    std::vector<KnapsackItem> items(n);
    for (int i = 0; i < n; ++i) {
        items[i] = {i, weights[i], values[i], static_cast<double>(values[i]) / weights[i]};
    }
    // Sort items by value/weight ratio descending
    std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) {
        return a.ratio > b.ratio;
    });
    return items;
}

// FIFO Branch and Bound (Queue-based)
std::pair<int, std::vector<int>> knapsackFIFO_BB(
    const std::vector<int>& weights,
    const std::vector<int>& values,
    int capacity) {
    int n = static_cast<int>(weights.size());
    if (n == 0 || capacity <= 0) return {0, {}};

    auto items = prepareItems(weights, values);
    std::queue<BBNode> Q;

    BBNode u, v;
    u.level = -1;
    u.profit = 0;
    u.weight = 0;
    u.bound = calculateBound(u, n, capacity, items);

    Q.push(u);
    int maxProfit = 0;
    std::vector<int> bestItems;

    while (!Q.empty()) {
        u = Q.front();
        Q.pop();

        if (u.level == n - 1) continue;
        if (u.bound <= maxProfit) continue;

        v.level = u.level + 1;

        // Branch 1: include item v.level
        v.weight = u.weight + items[v.level].weight;
        v.profit = u.profit + items[v.level].value;
        v.items = u.items;
        v.items.push_back(items[v.level].id);

        if (v.weight <= capacity && v.profit > maxProfit) {
            maxProfit = v.profit;
            bestItems = v.items;
        }

        v.bound = calculateBound(v, n, capacity, items);
        if (v.bound > maxProfit) {
            Q.push(v);
        }

        // Branch 2: exclude item v.level
        v.weight = u.weight;
        v.profit = u.profit;
        v.items = u.items;
        v.bound = calculateBound(v, n, capacity, items);
        if (v.bound > maxProfit) {
            Q.push(v);
        }
    }

    std::sort(bestItems.begin(), bestItems.end());
    return {maxProfit, bestItems};
}

// LIFO Branch and Bound (Stack-based)
std::pair<int, std::vector<int>> knapsackLIFO_BB(
    const std::vector<int>& weights,
    const std::vector<int>& values,
    int capacity) {
    int n = static_cast<int>(weights.size());
    if (n == 0 || capacity <= 0) return {0, {}};

    auto items = prepareItems(weights, values);
    std::stack<BBNode> S;

    BBNode u, v;
    u.level = -1;
    u.profit = 0;
    u.weight = 0;
    u.bound = calculateBound(u, n, capacity, items);

    S.push(u);
    int maxProfit = 0;
    std::vector<int> bestItems;

    while (!S.empty()) {
        u = S.top();
        S.pop();

        if (u.level == n - 1) continue;
        if (u.bound <= maxProfit) continue;

        v.level = u.level + 1;

        // Branch 2: exclude first (so include gets popped first in LIFO)
        v.weight = u.weight;
        v.profit = u.profit;
        v.items = u.items;
        v.bound = calculateBound(v, n, capacity, items);
        if (v.bound > maxProfit) {
            S.push(v);
        }

        // Branch 1: include
        v.weight = u.weight + items[v.level].weight;
        v.profit = u.profit + items[v.level].value;
        v.items = u.items;
        v.items.push_back(items[v.level].id);

        if (v.weight <= capacity && v.profit > maxProfit) {
            maxProfit = v.profit;
            bestItems = v.items;
        }

        v.bound = calculateBound(v, n, capacity, items);
        if (v.bound > maxProfit) {
            S.push(v);
        }
    }

    std::sort(bestItems.begin(), bestItems.end());
    return {maxProfit, bestItems};
}

// Least-Cost / Best-First Branch and Bound (Priority Queue)
struct CompareLC {
    bool operator()(const BBNode& a, const BBNode& b) const {
        return a.bound < b.bound; // Max-heap based on upper bound
    }
};

std::pair<int, std::vector<int>> knapsackLC_BB(
    const std::vector<int>& weights,
    const std::vector<int>& values,
    int capacity) {
    int n = static_cast<int>(weights.size());
    if (n == 0 || capacity <= 0) return {0, {}};

    auto items = prepareItems(weights, values);
    std::priority_queue<BBNode, std::vector<BBNode>, CompareLC> PQ;

    BBNode u, v;
    u.level = -1;
    u.profit = 0;
    u.weight = 0;
    u.bound = calculateBound(u, n, capacity, items);

    PQ.push(u);
    int maxProfit = 0;
    std::vector<int> bestItems;

    while (!PQ.empty()) {
        u = PQ.top();
        PQ.pop();

        if (u.bound <= maxProfit) continue;
        if (u.level == n - 1) continue;

        v.level = u.level + 1;

        // Include
        v.weight = u.weight + items[v.level].weight;
        v.profit = u.profit + items[v.level].value;
        v.items = u.items;
        v.items.push_back(items[v.level].id);

        if (v.weight <= capacity && v.profit > maxProfit) {
            maxProfit = v.profit;
            bestItems = v.items;
        }

        v.bound = calculateBound(v, n, capacity, items);
        if (v.bound > maxProfit) {
            PQ.push(v);
        }

        // Exclude
        v.weight = u.weight;
        v.profit = u.profit;
        v.items = u.items;
        v.bound = calculateBound(v, n, capacity, items);
        if (v.bound > maxProfit) {
            PQ.push(v);
        }
    }

    std::sort(bestItems.begin(), bestItems.end());
    return {maxProfit, bestItems};
}

// TSP Branch and Bound using state search
struct TSPNode {
    std::vector<int> path;
    std::vector<bool> visited;
    int curr;
    double cost;
    double lower_bound;
};

struct CompareTSPNode {
    bool operator()(const TSPNode& a, const TSPNode& b) const {
        return a.lower_bound > b.lower_bound;
    }
};

static double computeTSPLowerBound(int n, const std::vector<std::vector<double>>& dist,
                                   const std::vector<bool>& visited, int curr, double current_cost) {
    double lb = current_cost;
    for (int i = 0; i < n; ++i) {
        if (!visited[i] || i == curr) {
            double min_edge = 1e18;
            for (int j = 0; j < n; ++j) {
                if (i != j && (!visited[j] || j == 0)) {
                    min_edge = std::min(min_edge, dist[i][j]);
                }
            }
            if (min_edge < 1e18) lb += min_edge;
        }
    }
    return lb;
}

std::pair<double, std::vector<int>> tspBranchBound(
    const std::vector<std::vector<double>>& dist) {
    int n = static_cast<int>(dist.size());
    if (n <= 1) return {0.0, {0}};

    std::priority_queue<TSPNode, std::vector<TSPNode>, CompareTSPNode> pq;
    TSPNode root;
    root.curr = 0;
    root.path = {0};
    root.visited = std::vector<bool>(n, false);
    root.visited[0] = true;
    root.cost = 0.0;
    root.lower_bound = computeTSPLowerBound(n, dist, root.visited, 0, 0.0);
    pq.push(root);

    double bestCost = 1e18;
    std::vector<int> bestTour;

    while (!pq.empty()) {
        auto node = pq.top();
        pq.pop();

        if (node.lower_bound >= bestCost) continue;

        if (static_cast<int>(node.path.size()) == n) {
            // All visited, return to start
            double total_cost = node.cost + dist[node.curr][0];
            if (total_cost < bestCost) {
                bestCost = total_cost;
                bestTour = node.path;
                bestTour.push_back(0);
            }
            continue;
        }

        for (int next = 0; next < n; ++next) {
            if (!node.visited[next] && dist[node.curr][next] < 1e18) {
                TSPNode child;
                child.curr = next;
                child.path = node.path;
                child.path.push_back(next);
                child.visited = node.visited;
                child.visited[next] = true;
                child.cost = node.cost + dist[node.curr][next];
                child.lower_bound = computeTSPLowerBound(n, dist, child.visited, next, child.cost);

                if (child.lower_bound < bestCost) {
                    pq.push(child);
                }
            }
        }
    }

    return {bestCost, bestTour};
}

} // namespace sar

