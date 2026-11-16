/**
 * @file flow.cpp
 * @brief Implementations of Ford-Fulkerson (Edmonds-Karp) and Maximum Bipartite Matching.
 */

#include "../../include/flow.h"
#include <queue>
#include <algorithm>
#include <climits>

namespace sar {

FlowGraph::FlowGraph(int n) : n_(n), adj_(n) {}

void FlowGraph::addEdge(int from, int to, int capacity) {
    Edge forward{to, capacity, 0, static_cast<int>(adj_[to].size())};
    Edge backward{from, 0, 0, static_cast<int>(adj_[from].size())};
    adj_[from].push_back(forward);
    adj_[to].push_back(backward);
}

int FlowGraph::maxFlow(int source, int sink) {
    int total_flow = 0;

    while (true) {
        // BFS to find shortest augmenting path
        std::vector<int> parent_node(n_, -1);
        std::vector<int> parent_edge(n_, -1);
        std::queue<int> q;

        q.push(source);
        parent_node[source] = source;

        while (!q.empty() && parent_node[sink] == -1) {
            int u = q.front();
            q.pop();

            for (size_t i = 0; i < adj_[u].size(); ++i) {
                const auto& edge = adj_[u][i];
                if (parent_node[edge.to] == -1 && edge.capacity - edge.flow > 0) {
                    parent_node[edge.to] = u;
                    parent_edge[edge.to] = static_cast<int>(i);
                    q.push(edge.to);
                }
            }
        }

        if (parent_node[sink] == -1) break; // No augmenting path found

        // Find bottleneck residual capacity along path
        int push = INT_MAX;
        for (int v = sink; v != source; v = parent_node[v]) {
            int u = parent_node[v];
            int edge_idx = parent_edge[v];
            push = std::min(push, adj_[u][edge_idx].capacity - adj_[u][edge_idx].flow);
        }

        // Apply flow along path
        for (int v = sink; v != source; v = parent_node[v]) {
            int u = parent_node[v];
            int edge_idx = parent_edge[v];
            adj_[u][edge_idx].flow += push;
            int rev_idx = adj_[u][edge_idx].rev;
            adj_[v][rev_idx].flow -= push;
        }

        total_flow += push;
    }

    return total_flow;
}

// Bipartite matching via Augmenting Paths (Hopcroft-Karp / DFS)
static bool bpmDFS(int u, const std::vector<std::vector<int>>& adj,
                   std::vector<bool>& seen, std::vector<int>& matchR) {
    for (int v : adj[u]) {
        if (!seen[v]) {
            seen[v] = true;
            // If v is not assigned, or previously assigned right vertex can find another match
            if (matchR[v] < 0 || bpmDFS(matchR[v], adj, seen, matchR)) {
                matchR[v] = u;
                return true;
            }
        }
    }
    return false;
}

std::vector<std::pair<int, int>> maxBipartiteMatching(
    const std::vector<std::vector<int>>& adj,
    int nLeft, int nRight) {
    std::vector<int> matchR(nRight, -1);
    int matchCount = 0;

    for (int u = 0; u < nLeft; ++u) {
        std::vector<bool> seen(nRight, false);
        if (bpmDFS(u, adj, seen, matchR)) {
            matchCount++;
        }
    }

    std::vector<std::pair<int, int>> matching;
    matching.reserve(matchCount);
    for (int v = 0; v < nRight; ++v) {
        if (matchR[v] != -1) {
            matching.emplace_back(matchR[v], v);
        }
    }
    return matching;
}

} // namespace sar

