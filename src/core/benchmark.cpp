/**
 * @file benchmark.cpp
 * @brief Benchmark reporting implementation.
 */

#include "../../include/benchmark.h"

namespace sar {

void printBenchmarkTable(const std::vector<BenchmarkResult>& results) {
    std::cout << "\n========================================================================================\n";
    std::cout << std::left << std::setw(28) << "Algorithm"
              << std::setw(16) << "Dataset"
              << std::right << std::setw(16) << "Avg Time (us)"
              << std::setw(14) << "Time (ms)"
              << std::setw(12) << "Quality"
              << std::setw(10) << "Runs" << "\n";
    std::cout << "----------------------------------------------------------------------------------------\n";

    for (const auto& r : results) {
        std::cout << std::left << std::setw(28) << r.algorithm_name
                  << std::setw(16) << r.dataset_name
                  << std::right << std::fixed << std::setprecision(2)
                  << std::setw(16) << r.time_us
                  << std::setw(14) << (r.time_us / 1000.0)
                  << std::setw(12) << r.solution_quality
                  << std::setw(10) << r.iterations << "\n";
    }
    std::cout << "========================================================================================\n\n";
}

void compareBenchmarks(const std::vector<BenchmarkResult>& results) {
    if (results.size() < 2) return;
    std::cout << "\n--- Relative Speedup Comparison (Baseline: " << results[0].algorithm_name << ") ---\n";
    double baseline = results[0].time_us;
    if (baseline <= 0.0) baseline = 1e-9;

    for (size_t i = 1; i < results.size(); ++i) {
        double ratio = baseline / (results[i].time_us <= 0 ? 1e-9 : results[i].time_us);
        std::cout << "  * " << results[i].algorithm_name << " is "
                  << std::fixed << std::setprecision(2) << ratio << "x "
                  << (ratio >= 1.0 ? "faster" : "slower") << " than " << results[0].algorithm_name << "\n";
    }
    std::cout << "--------------------------------------------------------------------------------\n\n";
}

} // namespace sar

