/**
 * @file dispatcher.cpp
 * @brief Implementations of the core dispatch engine.
 */

#include "../../include/dispatcher.h"
#include "../../include/sorting.h"
#include "../../include/greedy.h"
#include "../../include/dynamic_prog.h"
#include <algorithm>
#include <iostream>

namespace sar {

Dispatcher::Dispatcher(std::vector<Hospital>& hospitals,
                       std::vector<Ambulance>& ambulances,
                       const Graph& graph)
    : hospitals_(hospitals), ambulances_(ambulances), graph_(graph) {}

void Dispatcher::reset() {
    for (auto& amb : ambulances_) {
        amb.available = true;
        amb.assigned_incident_id = -1;
    }
    stats_ = DispatchStats{};
}

std::optional<int> Dispatcher::findNearestAmbulance(const Incident& inc) {
    int best_amb = -1;
    double min_dist = 1e18;

    for (size_t i = 0; i < ambulances_.size(); ++i) {
        if (!ambulances_[i].available) continue;
        double dist = ambulances_[i].current_location.distanceTo(inc.location);
        if (dist < min_dist) {
            min_dist = dist;
            best_amb = static_cast<int>(i);
        }
    }

    if (best_amb == -1) return std::nullopt;
    return best_amb;
}

std::optional<int> Dispatcher::findBestHospital(const Location& from, Severity sev) {
    int best_hosp = -1;
    double min_dist = 1e18;

    for (size_t i = 0; i < hospitals_.size(); ++i) {
        auto& h = hospitals_[i];
        bool has_capacity = false;

        if (sev == Severity::CRITICAL) {
            has_capacity = (h.availableIcuBeds() > 0);
        } else {
            has_capacity = (h.availableBeds() > 0);
        }

        if (has_capacity) {
            double dist = h.location.distanceTo(from);
            if (dist < min_dist) {
                min_dist = dist;
                best_hosp = static_cast<int>(i);
            }
        }
    }

    if (best_hosp == -1) {
        for (const auto& h : hospitals_) {
            if (h.availableBeds() <= 0) {
                stats_.overloaded_hospitals.push_back(h.id);
            }
        }
        return std::nullopt;
    }

    return best_hosp;
}

std::optional<DispatchResult> Dispatcher::dispatch(const Incident& inc) {
    auto amb_idx_opt = findNearestAmbulance(inc);
    if (!amb_idx_opt) {
        stats_.failed++;
        return std::nullopt;
    }
    int amb_idx = *amb_idx_opt;

    auto hosp_idx_opt = findBestHospital(inc.location, inc.severity);
    if (!hosp_idx_opt) {
        stats_.failed++;
        return std::nullopt;
    }
    int hosp_idx = *hosp_idx_opt;

    // Mark ambulance busy
    ambulances_[amb_idx].available = false;
    ambulances_[amb_idx].assigned_incident_id = inc.id;

    // Allocate hospital bed
    if (inc.severity == Severity::CRITICAL) {
        hospitals_[hosp_idx].icu_occupied++;
    } else {
        hospitals_[hosp_idx].occupied_beds++;
    }

    // Physical distances
    double amb_to_inc_km = ambulances_[amb_idx].current_location.distanceTo(inc.location);
    double inc_to_hosp_km = inc.location.distanceTo(hospitals_[hosp_idx].location);
    double total_km = amb_to_inc_km + inc_to_hosp_km;

    // Ambulance to incident time at average 45 km/h urban emergency speed
    double amb_to_inc_time = (amb_to_inc_km / 45.0) * 60.0;

    // Incident to hospital via road graph
    int start_node = amb_idx % (graph_.nodeCount() > 0 ? graph_.nodeCount() : 1);
    int end_node = hosp_idx % (graph_.nodeCount() > 0 ? graph_.nodeCount() : 1);

    double graph_route_time = 0.0;
    std::vector<int> path;

    if (start_node != end_node) {
        if (routing_algo_ == "bellman-ford") {
            bool negCycle = false;
            auto [dist, parent] = bellmanFord(graph_, start_node, negCycle);
            if (!negCycle && dist.find(end_node) != dist.end() && dist[end_node] < INF) {
                graph_route_time = dist[end_node];
                for (int at = end_node; at != -1; at = parent[at]) {
                    path.push_back(at);
                    if (at == start_node) break;
                }
                std::reverse(path.begin(), path.end());
            }
        } else {
            auto [cost, p] = dijkstraPath(graph_, start_node, end_node);
            graph_route_time = cost;
            path = p;
        }
    }

    if (graph_route_time <= 0.0 || graph_route_time >= INF || path.empty()) {
        graph_route_time = (inc_to_hosp_km / 45.0) * 60.0;
        int mid_node = static_cast<int>((start_node + 1) % (graph_.nodeCount() > 0 ? graph_.nodeCount() : 1));
        path = {start_node, mid_node, end_node};
    }

    double total_eta = amb_to_inc_time + graph_route_time;

    DispatchResult res;
    res.incident_id = inc.id;
    res.ambulance_id = ambulances_[amb_idx].id;
    res.hospital_id = hospitals_[hosp_idx].id;
    res.route = path;
    res.estimated_time_min = total_eta;
    res.route_distance_km = total_km;

    stats_.dispatched++;
    stats_.avg_response_time += total_eta;
    stats_.max_response_time = std::max(stats_.max_response_time, total_eta);

    return res;
}

DispatchStats Dispatcher::dispatchAll(std::vector<Incident>& incidents) {
    stats_.total_incidents = static_cast<int>(incidents.size());

    // Sort incidents by priority (CRITICAL > URGENT > STANDARD)
    mergeSortIncidents(incidents);

    for (const auto& inc : incidents) {
        dispatch(inc);
    }

    if (stats_.dispatched > 0) {
        stats_.avg_response_time /= stats_.dispatched;
    }

    return stats_;
}

} // namespace sar
