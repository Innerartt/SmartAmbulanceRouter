#pragma once

#include <vector>
#include <optional>
#include <string>
#include "domain.h"
#include "graph.h"

namespace sar {

struct DispatchResult {
    int incident_id;
    int ambulance_id;
    int hospital_id;
    std::vector<int> route;       // Node sequence from ambulance to incident to hospital
    double estimated_time_min;
    double route_distance_km;
};

struct DispatchStats {
    int total_incidents = 0;
    int dispatched = 0;
    int failed = 0;
    double avg_response_time = 0.0;
    double max_response_time = 0.0;
    std::vector<int> overloaded_hospitals;
};

class Dispatcher {
public:
    Dispatcher(std::vector<Hospital>& hospitals,
               std::vector<Ambulance>& ambulances,
               const Graph& graph);

    // Attempt to dispatch an ambulance and select hospital for a single incident
    std::optional<DispatchResult> dispatch(const Incident& inc);

    // Batch dispatch a sequence of incidents (sorted by severity)
    DispatchStats dispatchAll(std::vector<Incident>& incidents);

    std::optional<int> findNearestAmbulance(const Incident& inc);
    std::optional<int> findBestHospital(const Location& from, Severity sev);

    const DispatchStats& getStats() const { return stats_; }
    void reset();

    void setAlgorithm(const std::string& algo_name) {
        routing_algo_ = algo_name;
    }

private:
    std::vector<Hospital>& hospitals_;
    std::vector<Ambulance>& ambulances_;
    const Graph& graph_;
    std::string routing_algo_ = "dijkstra";
    DispatchStats stats_;
};

} // namespace sar

