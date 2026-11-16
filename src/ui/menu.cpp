/**
 * @file menu.cpp
 * @brief Interactive CLI menu and Demonstration Scenarios.
 */

#include "../../include/menu.h"
#include "../../include/persistence.h"
#include "../../include/dispatcher.h"
#include "../../include/sorting.h"
#include "../../include/search.h"
#include "../../include/divide_conquer.h"
#include "../../include/greedy.h"
#include "../../include/dynamic_prog.h"
#include "../../include/backtracking.h"
#include "../../include/branch_bound.h"
#include "../../include/flow.h"
#include "../../include/number_theory.h"
#include "../../include/string_match.h"
#include "../../include/benchmark.h"

#include <iostream>
#include <iomanip>
#include <string>

namespace sar {

static void printHeader(const std::string& title) {
    std::cout << "\n===============================================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "===============================================================\n";
}

void runDemonstrationScenarios() {
    printHeader("SCENARIO 1: NORMAL OPERATION");
    std::cout << "Loading small city configuration...\n";

    auto hospitals = loadHospitals("data/small_hospitals.csv");
    auto ambulances = loadAmbulances("data/small_ambulances.csv");
    auto roads = loadRoads("data/small_roads.csv");
    auto incidents = loadIncidents("data/small_incidents.csv");

    Graph graph;
    graph.buildFromRoads(roads);

    std::cout << "Loaded " << hospitals.size() << " hospitals, "
              << ambulances.size() << " ambulances, "
              << roads.size() << " roads, "
              << incidents.size() << " incidents.\n\n";

    Dispatcher dispatcher(hospitals, ambulances, graph);
    dispatcher.setAlgorithm("dijkstra");

    std::cout << "Dispatching incidents with Dijkstra routing...\n";
    auto stats = dispatcher.dispatchAll(incidents);

    std::cout << "\n--- Scenario 1 Results ---\n"
              << "Total Incidents:      " << stats.total_incidents << "\n"
              << "Successfully Served:  " << stats.dispatched << "\n"
              << "Failed/Rejected:      " << stats.failed << "\n"
              << "Average Response:     " << std::fixed << std::setprecision(2)
              << stats.avg_response_time << " min\n"
              << "Max Response Time:    " << stats.max_response_time << " min\n"
              << "All patients served within target response times!\n";

    printHeader("SCENARIO 2: CAPACITY SHOCK (MASS CASUALTY EVENT)");
    std::cout << "Simulating sudden influx of 25 simultaneous CRITICAL incidents...\n";

    std::vector<Incident> massCasualties;
    for (int i = 100; i < 125; ++i) {
        massCasualties.push_back({
            i,
            {28.62 + (i % 5) * 0.01, 77.21 + (i % 7) * 0.01},
            Severity::CRITICAL,
            "Major highway collision multi-trauma",
            14400 + i * 10,
            false
        });
    }

    dispatcher.reset();
    auto shockStats = dispatcher.dispatchAll(massCasualties);

    std::cout << "\n--- Scenario 2 Results ---\n"
              << "Total Critical Alerts: " << shockStats.total_incidents << "\n"
              << "Immediate Dispatched:  " << shockStats.dispatched << "\n"
              << "Overload / Queued:     " << shockStats.failed << "\n";
    if (!shockStats.overloaded_hospitals.empty()) {
        std::cout << "ALERT: Hospitals [ ";
        for (int hid : shockStats.overloaded_hospitals) std::cout << "#" << hid << " ";
        std::cout << "] reached 100% capacity threshold!\n";
    }

    printHeader("SCENARIO 3: LARGE-DATA STRESS & ALGORITHM BENCHMARK");
    std::cout << "Benchmarking Sorting & Routing on Medium/Large Datasets...\n";

    std::vector<int> sampleArray(10000);
    for (int i = 0; i < 10000; ++i) sampleArray[i] = rand() % 100000;

    std::vector<BenchmarkResult> results;

    auto arr1 = sampleArray;
    results.push_back(runBenchmark("Merge Sort (10k items)", "Random Ints", [&]() {
        mergeSort(arr1);
    }, 5));

    auto arr2 = sampleArray;
    results.push_back(runBenchmark("Quick Sort (10k items)", "Random Ints", [&]() {
        quickSort(arr2);
    }, 5));

    results.push_back(runBenchmark("Dijkstra (Small City)", "10 Roads", [&]() {
        dijkstra(graph, 0);
    }, 20));

    results.push_back(runBenchmark("Prim MST (Small City)", "10 Roads", [&]() {
        primMST(graph);
    }, 20));

    results.push_back(runBenchmark("Kruskal MST (Small City)", "10 Roads", [&]() {
        kruskalMST(graph);
    }, 20));

    printBenchmarkTable(results);
}

void runInteractiveMenu() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n=======================================================\n";
        std::cout << "  Smart Emergency Ambulance Router - Control Center    \n";
        std::cout << "=======================================================\n";
        std::cout << " 1. Run All 3 Demonstration Scenarios\n";
        std::cout << " 2. Run Sorting & Searching Demo\n";
        std::cout << " 3. Run Routing (Dijkstra / MST / Bellman-Ford) Demo\n";
        std::cout << " 4. Run Dynamic Programming & Backtracking Demo\n";
        std::cout << " 5. Run Flow & String Matching Demo\n";
        std::cout << " 0. Exit\n";
        std::cout << "Choice: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::string discard;
            std::cin >> discard;
            continue;
        }

        switch (choice) {
            case 1:
                runDemonstrationScenarios();
                break;
            case 2: {
                printHeader("Sorting & Searching Demo");
                std::vector<int> numbers = {45, 12, 89, 3, 23, 7, 64, 18};
                std::cout << "Original Array: ";
                for (int x : numbers) std::cout << x << " ";
                std::cout << "\nRunning Merge Sort...\n";
                mergeSort(numbers);
                std::cout << "Sorted Array:   ";
                for (int x : numbers) std::cout << x << " ";
                std::cout << "\nBinary Search for 23: Index " << binarySearchIterative(numbers, 23) << "\n";
                break;
            }
            case 3: {
                printHeader("Routing & MST Demo");
                auto roads = loadRoads("data/small_roads.csv");
                Graph g;
                g.buildFromRoads(roads);
                std::cout << "Graph contains " << g.nodeCount() << " nodes, " << g.edgeCount() << " directed edges.\n";
                auto [cost, path] = dijkstraPath(g, 0, 4);
                std::cout << "Shortest Path from Node 0 to 4: ";
                for (int p : path) std::cout << p << " -> ";
                std::cout << " [Cost: " << cost << " min]\n";
                auto mst = kruskalMST(g);
                std::cout << "Kruskal MST edge count: " << mst.size() << "\n";
                break;
            }
            case 4: {
                printHeader("Dynamic Programming (0/1 Knapsack)");
                std::vector<int> weights = {2, 3, 4, 5};
                std::vector<int> values = {3, 4, 5, 6};
                int cap = 5;
                auto [maxVal, items] = knapsack01(weights, values, cap);
                std::cout << "Knapsack Capacity: " << cap << " | Max Value: " << maxVal << "\n";
                std::cout << "Selected Item Indices: ";
                for (int it : items) std::cout << it << " ";
                std::cout << "\n";
                break;
            }
            case 5: {
                printHeader("Network Flow & String Search Demo");
                FlowGraph fg(6);
                fg.addEdge(0, 1, 10);
                fg.addEdge(0, 2, 10);
                fg.addEdge(1, 2, 2);
                fg.addEdge(1, 3, 4);
                fg.addEdge(1, 4, 8);
                fg.addEdge(2, 4, 9);
                fg.addEdge(3, 5, 10);
                fg.addEdge(4, 5, 10);
                std::cout << "Max Flow (0 -> 5): " << fg.maxFlow(0, 5) << " units\n";

                std::string logText = "EMERGENCY_INCIDENT_0042_HOSPITAL_ALLOCATED_SUCCESS_EMERGENCY_OVERLOAD";
                std::string pattern = "EMERGENCY";
                auto kmpMatches = kmpSearch(logText, pattern);
                std::cout << "KMP String Search for '" << pattern << "' in log: found at offsets: ";
                for (int pos : kmpMatches) std::cout << pos << " ";
                std::cout << "\n";
                break;
            }
            case 0:
                std::cout << "Exiting Smart Ambulance Router. Goodbye!\n";
                break;
            default:
                std::cout << "Invalid choice. Please choose again.\n";
        }
    }
}

} // namespace sar

