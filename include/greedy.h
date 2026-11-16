#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <tuple>
#include <utility>
#include "graph.h"

namespace sar {

// Activity Selection: intervals as (start, finish)
std::vector<int> activitySelection(const std::vector<std::pair<int, int>>& intervals);

// Interval Partitioning: returns minimum number of resources (ambulances) needed
int intervalPartitioning(const std::vector<std::pair<int, int>>& intervals);

// Huffman Coding
struct HuffmanNode {
    char data;
    unsigned freq;
    HuffmanNode *left, *right;
    HuffmanNode(char d, unsigned f) : data(d), freq(f), left(nullptr), right(nullptr) {}
};

std::pair<std::string, std::unordered_map<char, std::string>> huffmanEncode(const std::string& text);
std::string huffmanDecode(const std::string& encoded, const std::unordered_map<char, std::string>& codeTable);

// Union-Find (Disjoint Set Union) for Kruskal's algorithm
class UnionFind {
public:
    explicit UnionFind(int n);
    int find(int i);
    bool unite(int i, int j);
    bool connected(int i, int j);

private:
    std::vector<int> parent_;
    std::vector<int> rank_;
};

// Dijkstra's Shortest Path
// Returns (distances mapped from node id, parent pointers for path reconstruction)
std::pair<std::unordered_map<int, double>, std::unordered_map<int, int>> dijkstra(const Graph& g, int source);
std::pair<double, std::vector<int>> dijkstraPath(const Graph& g, int source, int dest);

// Prim's Minimum Spanning Tree: returns vector of (u, v, weight)
std::vector<std::tuple<int, int, double>> primMST(const Graph& g);

// Kruskal's Minimum Spanning Tree: returns vector of (u, v, weight)
std::vector<std::tuple<int, int, double>> kruskalMST(const Graph& g);

} // namespace sar

