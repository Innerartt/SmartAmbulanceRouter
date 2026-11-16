#pragma once

/// @file graph.h
/// @brief Weighted graph ADT for road-network modelling in the Smart Ambulance Router.

#include <algorithm>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "domain.h"

namespace sar {

/// Sentinel value representing "no edge" / infinite distance.
inline constexpr double INF = 1e18;

/**
 * @class Graph
 * @brief Integer-keyed, weighted, directed (optionally bidirectional) graph
 *        stored as an adjacency list.
 *
 * Nodes are identified by plain `int` IDs that correspond to the
 * `from_node` / `to_node` fields of `Road` structs.  Edge weights are
 * `double` values (typically `travel_time_min * traffic_factor`).
 */
class Graph {
public:
    // ---------------------------------------------------------------
    //  Construction / reset
    // ---------------------------------------------------------------

    Graph() = default;

    /**
     * @brief Populate the graph from a vector of Road structs.
     *
     * Each road is added as a **bidirectional** edge whose weight equals
     * `travel_time_min * traffic_factor` (effective travel time).
     *
     * @param roads  Collection of roads to import.
     */
    void buildFromRoads(const std::vector<Road>& roads) {
        clear();
        for (const auto& road : roads) {
            addNode(road.from_node);
            addNode(road.to_node);
            // Effective weight accounts for current traffic conditions.
            const double weight = road.travel_time_min * road.traffic_factor;
            addEdge(road.from_node, road.to_node, weight, /*bidirectional=*/true);
        }
    }

    /**
     * @brief Remove every node and edge, resetting the graph to an empty state.
     */
    void clear() {
        adj_.clear();
        nodes_.clear();
        edge_count_ = 0;
    }

    // ---------------------------------------------------------------
    //  Node operations
    // ---------------------------------------------------------------

    /**
     * @brief Insert a node. No-op if the node already exists.
     * @param id  Unique integer identifier for the node.
     */
    void addNode(int id) {
        if (nodes_.find(id) == nodes_.end()) {
            nodes_.insert(id);
            adj_.emplace(id, std::vector<std::pair<int, double>>{});
        }
    }

    /**
     * @brief Check whether a node with the given id exists.
     * @param id  Node identifier.
     * @return `true` if the node is present.
     */
    [[nodiscard]] bool hasNode(int id) const {
        return nodes_.count(id) > 0;
    }

    /**
     * @brief Return a sorted vector of every node ID in the graph.
     * @return Vector of node IDs.
     */
    [[nodiscard]] std::vector<int> getNodes() const {
        std::vector<int> result(nodes_.begin(), nodes_.end());
        std::sort(result.begin(), result.end());
        return result;
    }

    /**
     * @brief Number of nodes currently in the graph.
     */
    [[nodiscard]] std::size_t nodeCount() const noexcept {
        return nodes_.size();
    }

    // ---------------------------------------------------------------
    //  Edge operations
    // ---------------------------------------------------------------

    /**
     * @brief Add a weighted edge (and optionally its reverse).
     *
     * If an edge `from -> to` already exists it is **not** duplicated;
     * its weight is updated in-place instead.
     *
     * Both endpoint nodes are created automatically if they are absent.
     *
     * @param from            Source node.
     * @param to              Destination node.
     * @param weight          Edge weight (e.g. travel time in minutes).
     * @param bidirectional   If `true`, an edge `to -> from` with the same
     *                        weight is added as well.
     */
    void addEdge(int from, int to, double weight, bool bidirectional = true) {
        addNode(from);
        addNode(to);

        // Helper: insert or update a single directed edge.
        auto insertOrUpdate = [this](int src, int dst, double w) {
            auto& neighbours = adj_[src];
            auto it = std::find_if(neighbours.begin(), neighbours.end(),
                                   [dst](const auto& p) { return p.first == dst; });
            if (it != neighbours.end()) {
                it->second = w;          // update existing weight
            } else {
                neighbours.emplace_back(dst, w);
                ++edge_count_;
            }
        };

        insertOrUpdate(from, to, weight);
        if (bidirectional) {
            insertOrUpdate(to, from, weight);
        }
    }

    /**
     * @brief Remove the directed edge `from -> to` (if it exists).
     * @param from  Source node.
     * @param to    Destination node.
     */
    void removeEdge(int from, int to) {
        auto it = adj_.find(from);
        if (it == adj_.end()) return;

        auto& neighbours = it->second;
        auto edge_it = std::find_if(neighbours.begin(), neighbours.end(),
                                    [to](const auto& p) { return p.first == to; });
        if (edge_it != neighbours.end()) {
            neighbours.erase(edge_it);
            --edge_count_;
        }
    }

    /**
     * @brief Check whether a directed edge `from -> to` exists.
     * @return `true` if the edge is present.
     */
    [[nodiscard]] bool hasEdge(int from, int to) const {
        auto it = adj_.find(from);
        if (it == adj_.end()) return false;

        const auto& neighbours = it->second;
        return std::any_of(neighbours.cbegin(), neighbours.cend(),
                           [to](const auto& p) { return p.first == to; });
    }

    /**
     * @brief Look up the weight of the directed edge `from -> to`.
     * @return The weight wrapped in `std::optional`, or `std::nullopt`
     *         if the edge does not exist.
     */
    [[nodiscard]] std::optional<double> getWeight(int from, int to) const {
        auto it = adj_.find(from);
        if (it == adj_.end()) return std::nullopt;

        const auto& neighbours = it->second;
        auto edge_it = std::find_if(neighbours.cbegin(), neighbours.cend(),
                                    [to](const auto& p) { return p.first == to; });
        if (edge_it != neighbours.cend()) {
            return edge_it->second;
        }
        return std::nullopt;
    }

    /**
     * @brief Return the adjacency list of a given node.
     * @param node  Node whose neighbours are requested.
     * @return Vector of `(neighbour_id, weight)` pairs.
     *         An empty vector is returned if the node does not exist.
     */
    [[nodiscard]] std::vector<std::pair<int, double>> getNeighbors(int node) const {
        auto it = adj_.find(node);
        if (it != adj_.end()) {
            return it->second;
        }
        return {};
    }

    /**
     * @brief Total number of directed edges in the graph.
     *
     * A bidirectional road counts as **two** directed edges.
     */
    [[nodiscard]] std::size_t edgeCount() const noexcept {
        return edge_count_;
    }

    // ---------------------------------------------------------------
    //  Matrix representation
    // ---------------------------------------------------------------

    /**
     * @brief Build a dense adjacency matrix.
     *
     * Rows and columns are ordered by the **sorted** node IDs returned by
     * `getNodes()`.  `matrix[i][j]` holds the edge weight from the i-th
     * node to the j-th node, or `INF` when no direct edge exists.
     * Diagonal entries are `0.0`.
     *
     * @return 2-D vector of doubles sized `nodeCount() × nodeCount()`.
     */
    [[nodiscard]] std::vector<std::vector<double>> getAdjacencyMatrix() const {
        const auto node_ids = getNodes();              // sorted
        const auto n = node_ids.size();

        // Map each node ID to its matrix index.
        std::unordered_map<int, std::size_t> index_of;
        index_of.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            index_of[node_ids[i]] = i;
        }

        // Initialise with INF, diagonal with 0.
        std::vector<std::vector<double>> matrix(n, std::vector<double>(n, INF));
        for (std::size_t i = 0; i < n; ++i) {
            matrix[i][i] = 0.0;
        }

        // Fill in actual weights.
        for (const auto& [src, neighbours] : adj_) {
            const auto row = index_of.at(src);
            for (const auto& [dst, weight] : neighbours) {
                const auto col = index_of.at(dst);
                matrix[row][col] = weight;
            }
        }

        return matrix;
    }

private:
    /// Adjacency list: node-id → [(neighbour-id, weight), …]
    std::unordered_map<int, std::vector<std::pair<int, double>>> adj_;

    /// Set of all node IDs (kept in sync with adj_ keys).
    std::unordered_set<int> nodes_;

    /// Running count of directed edges.
    std::size_t edge_count_ = 0;
};

} // namespace sar
