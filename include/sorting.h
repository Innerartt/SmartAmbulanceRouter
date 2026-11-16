#pragma once

#include <vector>
#include <functional>
#include <algorithm>
#include "domain.h"

namespace sar {

// Generic template Merge Sort
template <typename T, typename Compare = std::less<T>>
void mergeSort(std::vector<T>& arr, Compare comp = Compare()) {
    if (arr.size() <= 1) return;

    std::function<void(int, int)> mergeSortHelper = [&](int left, int right) {
        if (left >= right) return;
        int mid = left + (right - left) / 2;
        mergeSortHelper(left, mid);
        mergeSortHelper(mid + 1, right);

        std::vector<T> temp;
        temp.reserve(right - left + 1);
        int i = left, j = mid + 1;
        while (i <= mid && j <= right) {
            if (comp(arr[i], arr[j])) {
                temp.push_back(arr[i++]);
            } else {
                temp.push_back(arr[j++]);
            }
        }
        while (i <= mid) temp.push_back(arr[i++]);
        while (j <= right) temp.push_back(arr[j++]);

        for (int k = 0; k < static_cast<int>(temp.size()); ++k) {
            arr[left + k] = temp[k];
        }
    };

    mergeSortHelper(0, static_cast<int>(arr.size()) - 1);
}

// Generic template Quick Sort
template <typename T, typename Compare = std::less<T>>
void quickSort(std::vector<T>& arr, Compare comp = Compare()) {
    if (arr.size() <= 1) return;

    std::function<void(int, int)> quickSortHelper = [&](int low, int high) {
        if (low >= high) return;

        // Randomized pivot
        int pivotIndex = low + rand() % (high - low + 1);
        std::swap(arr[pivotIndex], arr[high]);
        const T& pivot = arr[high];

        int i = low - 1;
        for (int j = low; j < high; ++j) {
            if (comp(arr[j], pivot)) {
                ++i;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]);
        int pi = i + 1;

        quickSortHelper(low, pi - 1);
        quickSortHelper(pi + 1, high);
    };

    quickSortHelper(0, static_cast<int>(arr.size()) - 1);
}

// Domain-specific sorting
void mergeSortIncidents(std::vector<Incident>& incidents);
void quickSortAmbulances(std::vector<Ambulance>& ambulances, const Location& target);

} // namespace sar

