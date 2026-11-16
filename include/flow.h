#pragma once

#include <vector>
#include <utility>

namespace sar {

class FlowGraph {
public:
    explicit FlowGraph(int n);
    void addEdge(int from, int to, int capacity);
    int maxFlow(int source, int sink);

    struct Edge {
        int to;
        int capacity;
        int flow;
        int rev; // index of reverse edge in adj[to]
    };

    const std::vector<Edge>& getEdges(int u) const { return adj_[u]; }

private:
    int n_;
    std::vector<std::vector<Edge>> adj_;
};

// Maximum Bipartite Matching:
// adj: adj[u] contains list of right-partition vertices connected to u
// nLeft: number of vertices in left set (0..nLeft-1)
// nRight: number of vertices in right set (0..nRight-1)
// Returns list of (left_u, right_v) matched pairs
std::vector<std::pair<int, int>> maxBipartiteMatching(
    const std::vector<std::vector<int>>& adj,
    int nLeft, int nRight);

} // namespace sar

