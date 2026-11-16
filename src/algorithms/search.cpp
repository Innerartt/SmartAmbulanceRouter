/**
 * @file search.cpp
 * @brief Implementations of domain-specific search algorithms.
 */

#include "../../include/search.h"

namespace sar {

int findHospitalByCapacity(const std::vector<Hospital>& sortedHospitals, int minBeds) {
    // Binary search (lower bound) for first hospital with availableBeds >= minBeds
    int low = 0;
    int high = static_cast<int>(sortedHospitals.size()) - 1;
    int ans = -1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (sortedHospitals[mid].availableBeds() >= minBeds) {
            ans = mid;
            high = mid - 1; // look for earlier valid entry
        } else {
            low = mid + 1;
        }
    }
    return ans;
}

int findAmbulanceById(const std::vector<Ambulance>& sortedAmbulances, int id) {
    return binarySearchIterative(sortedAmbulances, id, [](const Ambulance& amb) {
        return amb.id;
    });
}

} // namespace sar

