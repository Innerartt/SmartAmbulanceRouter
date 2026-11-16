#pragma once

#include <chrono>
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <iomanip>

namespace sar {

class Timer {
public:
    void start() {
        start_time_ = std::chrono::high_resolution_clock::now();
        running_ = true;
    }

    void stop() {
        end_time_ = std::chrono::high_resolution_clock::now();
        running_ = false;
    }

    double elapsedMicroseconds() const {
        auto end = running_ ? std::chrono::high_resolution_clock::now() : end_time_;
        return std::chrono::duration<double, std::micro>(end - start_time_).count();
    }

    double elapsedMilliseconds() const {
        return elapsedMicroseconds() / 1000.0;
    }

    double elapsedSeconds() const {
        return elapsedMicroseconds() / 1000000.0;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
    std::chrono::time_point<std::chrono::high_resolution_clock> end_time_;
    bool running_ = false;
};

struct BenchmarkResult {
    std::string algorithm_name;
    std::string dataset_name;
    double time_us;          // average runtime in microseconds
    double peak_memory_kb;    // estimated memory
    double solution_quality;  // domain-specific quality metric
    int iterations;
};

template <typename Func>
BenchmarkResult runBenchmark(const std::string& algo_name,
                            const std::string& dataset_name,
                            Func&& fn,
                            int iterations = 10,
                            double solution_quality = 1.0) {
    Timer timer;
    double total_us = 0.0;

    for (int i = 0; i < iterations; ++i) {
        timer.start();
        fn();
        timer.stop();
        total_us += timer.elapsedMicroseconds();
    }

    BenchmarkResult res;
    res.algorithm_name = algo_name;
    res.dataset_name = dataset_name;
    res.time_us = total_us / iterations;
    res.peak_memory_kb = 0.0; // placeholder
    res.solution_quality = solution_quality;
    res.iterations = iterations;
    return res;
}

void printBenchmarkTable(const std::vector<BenchmarkResult>& results);
void compareBenchmarks(const std::vector<BenchmarkResult>& results);

} // namespace sar

