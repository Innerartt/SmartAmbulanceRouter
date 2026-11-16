#pragma once

#include <vector>
#include <functional>
#include "domain.h"

namespace sar {

// Generic iterative binary search: returns index or -1
template <typename T, typename Key, typename Extractor = std::identity>
int binarySearchIterative(const std::vector<T>& arr, const Key& key, Extractor ext = Extractor{}) {
    int low = 0;
    int high = static_cast<int>(arr.size()) - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        auto val = ext(arr[mid]);
        if (val == key) return mid;
        if (val < key) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

// Generic recursive binary search helper
template <typename T, typename Key, typename Extractor = std::identity>
int binarySearchRecursive(const std::vector<T>& arr, const Key& key, int low, int high, Extractor ext = Extractor{}) {
    if (low > high) return -1;
    int mid = low + (high - low) / 2;
    auto val = ext(arr[mid]);
    if (val == key) return mid;
    if (val < key) {
        return binarySearchRecursive(arr, key, mid + 1, high, ext);
    } else {
        return binarySearchRecursive(arr, key, low, mid - 1, ext);
    }
}

// Domain-specific searches
int findHospitalByCapacity(const std::vector<Hospital>& sortedHospitals, int minBeds);
int findAmbulanceById(const std::vector<Ambulance>& sortedAmbulances, int id);

} // namespace sar

