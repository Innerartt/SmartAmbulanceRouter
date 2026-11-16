/**
 * @file greedy.cpp
 * @brief Implementations of Greedy algorithms.
 */

#include "../../include/greedy.h"
#include <queue>
#include <algorithm>
#include <numeric>

namespace sar {

std::vector<int> activitySelection(const std::vector<std::pair<int, int>>& intervals) {
    if (intervals.empty()) return {};

    // Store index along with interval
    std::vector<std::tuple<int, int, int>> acts;
    acts.reserve(intervals.size());
    for (size_t i = 0; i < intervals.size(); ++i) {
        acts.emplace_back(intervals[i].first, intervals[i].second, static_cast<int>(i));
    }

    // Sort by finish time ascending
    std::sort(acts.begin(), acts.end(), [](const auto& a, const auto& b) {
        return std::get<1>(a) < std::get<1>(b);
    });

    std::vector<int> selected;
    selected.push_back(std::get<2>(acts[0]));
    int lastFinish = std::get<1>(acts[0]);

    for (size_t i = 1; i < acts.size(); ++i) {
        if (std::get<0>(acts[i]) >= lastFinish) {
            selected.push_back(std::get<2>(acts[i]));
            lastFinish = std::get<1>(acts[i]);
        }
    }
    return selected;
}

int intervalPartitioning(const std::vector<std::pair<int, int>>& intervals) {
    if (intervals.empty()) return 0;

    std::vector<std::pair<int, int>> sorted = intervals;
    // Sort by start time
    std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
    });

    // Min-heap storing finish times of ongoing partitions
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;

    for (const auto& act : sorted) {
        if (!minHeap.empty() && minHeap.top() <= act.first) {
            minHeap.pop(); // Reuse room/ambulance
        }
        minHeap.push(act.second);
    }

    return static_cast<int>(minHeap.size());
}

// Huffman Coding
struct CompareHuffman {
    bool operator()(HuffmanNode* l, HuffmanNode* r) {
        return l->freq > r->freq;
    }
};

static void buildHuffmanCodes(HuffmanNode* root, const std::string& str,
                              std::unordered_map<char, std::string>& codes) {
    if (!root) return;
    if (!root->left && !root->right) {
        codes[root->data] = str.empty() ? "0" : str;
    }
    buildHuffmanCodes(root->left, str + "0", codes);
    buildHuffmanCodes(root->right, str + "1", codes);
}

static void freeHuffmanTree(HuffmanNode* root) {
    if (!root) return;
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    delete root;
}

std::pair<std::string, std::unordered_map<char, std::string>> huffmanEncode(const std::string& text) {
    if (text.empty()) return {"", {}};

    std::unordered_map<char, unsigned> freqMap;
    for (char c : text) freqMap[c]++;

    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareHuffman> pq;
    for (auto& pair : freqMap) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    while (pq.size() > 1) {
        auto left = pq.top(); pq.pop();
        auto right = pq.top(); pq.pop();
        auto top = new HuffmanNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        pq.push(top);
    }

    HuffmanNode* root = pq.top();
    std::unordered_map<char, std::string> codeTable;
    buildHuffmanCodes(root, "", codeTable);
    freeHuffmanTree(root);

    std::string encoded;
    for (char c : text) {
        encoded += codeTable[c];
    }

    return {encoded, codeTable};
}

std::string huffmanDecode(const std::string& encoded, const std::unordered_map<char, std::string>& codeTable) {
    std::unordered_map<std::string, char> reverseTable;
    for (const auto& pair : codeTable) {
        reverseTable[pair.second] = pair.first;
    }

    std::string decoded;
    std::string current;
    for (char bit : encoded) {
        current += bit;
        auto it = reverseTable.find(current);
        if (it != reverseTable.end()) {
            decoded += it->second;
            current.clear();
        }
    }
    return decoded;
}

// Union-Find
UnionFind::UnionFind(int n) : parent_(n), rank_(n, 0) {
    std::iota(parent_.begin(), parent_.end(), 0);
}

int UnionFind::find(int i) {
    if (parent_[i] == i) return i;
    return parent_[i] = find(parent_[i]);
}

bool UnionFind::unite(int i, int j) {
    int root_i = find(i);
    int root_j = find(j);
    if (root_i == root_j) return false;

    if (rank_[root_i] < rank_[root_j]) {
        parent_[root_i] = root_j;
    } else if (rank_[root_i] > rank_[root_j]) {
        parent_[root_j] = root_i;
    } else {
        parent_[root_j] = root_i;
        rank_[root_i]++;
    }
    return true;
}

bool UnionFind::connected(int i, int j) {
    return find(i) == find(j);
}

// Dijkstra
std::pair<std::unordered_map<int, double>, std::unordered_map<int, int>> dijkstra(const Graph& g, int source) {
    std::unordered_map<int, double> dist;
    std::unordered_map<int, int> parent;

    for (int node : g.getNodes()) {
        dist[node] = INF;
        parent[node] = -1;
    }
    if (!g.hasNode(source)) return {dist, parent};

    dist[source] = 0.0;
    using PDI = std::pair<double, int>;
    std::priority_queue<PDI, std::vector<PDI>, std::greater<PDI>> pq;
    pq.push({0.0, source});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;

        for (const auto& edge : g.getNeighbors(u)) {
            int v = edge.first;
            double weight = edge.second;
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    return {dist, parent};
}

std::pair<double, std::vector<int>> dijkstraPath(const Graph& g, int source, int dest) {
    auto [dist, parent] = dijkstra(g, source);
    if (dist.find(dest) == dist.end() || dist[dest] >= INF) {
        return {INF, {}};
    }

    std::vector<int> path;
    for (int at = dest; at != -1; at = parent[at]) {
        path.push_back(at);
        if (at == source) break;
    }
    std::reverse(path.begin(), path.end());
    if (path.empty() || path.front() != source) return {INF, {}};
    return {dist[dest], path};
}

// Prim's MST
std::vector<std::tuple<int, int, double>> primMST(const Graph& g) {
    std::vector<std::tuple<int, int, double>> mst;
    auto nodes = g.getNodes();
    if (nodes.empty()) return mst;

    std::unordered_set<int> inMST;
    using EdgeTuple = std::tuple<double, int, int>; // (weight, from, to)
    std::priority_queue<EdgeTuple, std::vector<EdgeTuple>, std::greater<EdgeTuple>> pq;

    int startNode = nodes[0];
    inMST.insert(startNode);
    for (const auto& edge : g.getNeighbors(startNode)) {
        pq.push({edge.second, startNode, edge.first});
    }

    while (!pq.empty() && inMST.size() < nodes.size()) {
        auto [w, u, v] = pq.top();
        pq.pop();

        if (inMST.count(v)) continue;

        inMST.insert(v);
        mst.emplace_back(u, v, w);

        for (const auto& edge : g.getNeighbors(v)) {
            if (!inMST.count(edge.first)) {
                pq.push({edge.second, v, edge.first});
            }
        }
    }
    return mst;
}

// Kruskal's MST
std::vector<std::tuple<int, int, double>> kruskalMST(const Graph& g) {
    std::vector<std::tuple<int, int, double>> mst;
    auto nodes = g.getNodes();
    if (nodes.empty()) return mst;

    // Map arbitrary node IDs to 0..N-1
    std::unordered_map<int, int> nodeToIdx;
    std::unordered_map<int, int> idxToNode;
    for (size_t i = 0; i < nodes.size(); ++i) {
        nodeToIdx[nodes[i]] = static_cast<int>(i);
        idxToNode[static_cast<int>(i)] = nodes[i];
    }

    // Collect all edges
    std::vector<std::tuple<double, int, int>> edges;
    for (int u : nodes) {
        for (const auto& edge : g.getNeighbors(u)) {
            int v = edge.first;
            if (u < v) { // undirected graph avoiding duplicate edges
                edges.emplace_back(edge.second, u, v);
            }
        }
    }

    std::sort(edges.begin(), edges.end());
    UnionFind uf(static_cast<int>(nodes.size()));

    for (const auto& [w, u, v] : edges) {
        int uIdx = nodeToIdx[u];
        int vIdx = nodeToIdx[v];
        if (uf.unite(uIdx, vIdx)) {
            mst.emplace_back(u, v, w);
            if (mst.size() == nodes.size() - 1) break;
        }
    }
    return mst;
}

} // namespace sar

