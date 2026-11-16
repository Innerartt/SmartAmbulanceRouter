/**
 * @file sorting.cpp
 * @brief Implementations of domain-specific sorting routines.
 */

#include "../../include/sorting.h"

namespace sar {

void mergeSortIncidents(std::vector<Incident>& incidents) {
    // Sort primarily by severity (CRITICAL < URGENT < STANDARD), then by timestamp ascending
    mergeSort(incidents, [](const Incident& a, const Incident& b) {
        if (a.severity != b.severity) {
            return static_cast<int>(a.severity) < static_cast<int>(b.severity);
        }
        if (a.timestamp != b.timestamp) {
            return a.timestamp < b.timestamp;
        }
        return a.id < b.id;
    });
}

void quickSortAmbulances(std::vector<Ambulance>& ambulances, const Location& target) {
    // Sort ambulances by increasing Haversine distance to the target location
    quickSort(ambulances, [&](const Ambulance& a, const Ambulance& b) {
        double distA = a.current_location.distanceTo(target);
        double distB = b.current_location.distanceTo(target);
        if (distA != distB) {
            return distA < distB;
        }
        return a.id < b.id;
    });
}

} // namespace sar

