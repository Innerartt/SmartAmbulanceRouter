/**
 * @file menu.cpp
 * @brief Interactive CLI menu, Live Simulation Engine, and Demonstration Scenarios.
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
#include <vector>

namespace sar {

static void printHeader(const std::string& title) {
    std::cout << "\n====================================================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "====================================================================\n";
}

void runLiveSimulation() {
    printHeader("LIVE EMERGENCY DISPATCH SIMULATOR");
    std::cout << "Loading Metropolitan Emergency System Data...\n";

    auto hospitals = loadHospitals("data/small_hospitals.csv");
    auto ambulances = loadAmbulances("data/small_ambulances.csv");
    auto roads = loadRoads("data/small_roads.csv");
    auto incidents = loadIncidents("data/small_incidents.csv");

    Graph graph;
    graph.buildFromRoads(roads);

    std::cout << "Successfully loaded:\n"
              << "  * " << hospitals.size() << " Medical Centers with Trauma & ICU infrastructure\n"
              << "  * " << ambulances.size() << " Emergency Ambulances on Active Patrol\n"
              << "  * " << roads.size() << " Monitored Road Segments\n"
              << "  * " << incidents.size() << " Incoming 911/108 Emergency Incidents\n\n";

    std::cout << "Initial Hospital Bed Status:\n";
    std::cout << "--------------------------------------------------------------------\n";
    std::cout << std::left << std::setw(5) << "ID"
              << std::setw(28) << "Hospital Name"
              << std::setw(18) << "General Beds"
              << std::setw(15) << "ICU Beds" << "\n";
    std::cout << "--------------------------------------------------------------------\n";
    for (const auto& h : hospitals) {
        std::cout << std::left << std::setw(5) << h.id
                  << std::setw(28) << h.name
                  << (std::to_string(h.availableBeds()) + " / " + std::to_string(h.total_beds))
                  << std::right << std::setw(14)
                  << (std::to_string(h.availableIcuBeds()) + " / " + std::to_string(h.icu_beds))
                  << "\n";
    }
    std::cout << "--------------------------------------------------------------------\n";

    std::cout << "\n[Step 1] Prioritizing Emergency Queue using Merge Sort (O(n log n))...\n";
    mergeSortIncidents(incidents);
    std::cout << "Sorted Queue: Critical cases placed at front of dispatch queue.\n\n";

    Dispatcher dispatcher(hospitals, ambulances, graph);
    dispatcher.setAlgorithm("dijkstra");

    std::cout << "[Step 2] Processing Dispatch Queue with Real-Time Routing...\n";
    std::cout << "====================================================================\n";

    int event_num = 1;
    for (const auto& inc : incidents) {
        std::cout << "\n>>> [INCIDENT #" << event_num++ << "] "
                  << "Severity: [" << severityToString(inc.severity) << "] "
                  << (inc.severity == Severity::CRITICAL ? "🚨" : (inc.severity == Severity::URGENT ? "⚠️" : "ℹ️"))
                  << " \"" << inc.description << "\"\n"
                  << "    Location: (" << inc.location.latitude << ", " << inc.location.longitude << ") | Time: "
                  << inc.timestamp << "s\n";

        auto result = dispatcher.dispatch(inc);
        if (result) {
            std::cout << "    → Assigned Ambulance: Unit #" << result->ambulance_id
                      << " (Capacity Level: " << ambulances[result->ambulance_id].capacity << ")\n";

            int hid = result->hospital_id;
            std::cout << "    → Destination Hospital: \"" << hospitals[hid].name << "\" [Node #" << hid << "]\n";
            std::cout << "    → Remaining Beds: General: " << hospitals[hid].availableBeds()
                      << " | ICU: " << hospitals[hid].availableIcuBeds() << "\n";

            std::cout << "    → Dijkstra Computed Route: [ ";
            for (size_t i = 0; i < result->route.size(); ++i) {
                std::cout << result->route[i] << (i + 1 < result->route.size() ? " -> " : " ");
            }
            std::cout << "]\n";

            std::cout << "    → Estimated Transit Time: " << std::fixed << std::setprecision(2)
                      << result->estimated_time_min << " min ("
                      << result->route_distance_km << " km)\n";
            std::cout << "    ✓ STATUS: AMBULANCE DISPATCHED & BED RESERVED\n";
        } else {
            std::cout << "    ❌ STATUS: OVERLOAD WARNING — NO SUITABLE BED/AMBULANCE AVAILABLE!\n";
        }
    }

    auto stats = dispatcher.getStats();
    stats.total_incidents = static_cast<int>(incidents.size());
    if (stats.dispatched > 0) {
        stats.avg_response_time /= stats.dispatched;
    }
    printHeader("SIMULATION RESULTS & PERFORMANCE METRICS");
    std::cout << "  * Total Emergency Calls:    " << stats.total_incidents << "\n"
              << "  * Successfully Dispatched:  " << stats.dispatched << " ("
              << (stats.dispatched * 100.0 / stats.total_incidents) << "%)\n"
              << "  * Overload / Rejected:      " << stats.failed << "\n"
              << "  * Average Response Time:    " << std::fixed << std::setprecision(2)
              << stats.avg_response_time << " minutes\n"
              << "  * Maximum Response Time:    " << stats.max_response_time << " minutes\n";
    std::cout << "====================================================================\n\n";
}

void runDemonstrationScenarios() {
    runLiveSimulation();

    printHeader("CAPACITY SHOCK (MASS CASUALTY SIMULATION)");
    std::cout << "Simulating sudden disaster influx: 25 simultaneous CRITICAL patients...\n";

    auto hospitals = loadHospitals("data/small_hospitals.csv");
    auto ambulances = loadAmbulances("data/small_ambulances.csv");
    auto roads = loadRoads("data/small_roads.csv");
    Graph graph;
    graph.buildFromRoads(roads);

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

    Dispatcher dispatcher(hospitals, ambulances, graph);
    auto shockStats = dispatcher.dispatchAll(massCasualties);

    std::cout << "\n--- Mass Casualty Results ---\n"
              << "Total Critical Alerts: " << shockStats.total_incidents << "\n"
              << "Immediate Dispatched:  " << shockStats.dispatched << "\n"
              << "Overload / Queued:     " << shockStats.failed << "\n";
    if (!shockStats.overloaded_hospitals.empty()) {
        std::cout << "OVERLOAD ALERT: Capacity reached for Trauma/ICU beds!\n";
    }

    printHeader("ALGORITHM BENCHMARK EXECUTION");
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

    results.push_back(runBenchmark("Dijkstra (City Graph)", "10 Roads", [&]() {
        dijkstra(graph, 0);
    }, 20));

    printBenchmarkTable(results);
}

void runInteractiveMenu() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n=======================================================\n";
        std::cout << "  Smart Emergency Ambulance Router - Control Center    \n";
        std::cout << "=======================================================\n";
        std::cout << " 1. Run Live Emergency Simulation (Step-by-Step)\n";
        std::cout << " 2. Run All Demonstration Scenarios & Benchmarks\n";
        std::cout << " 3. Run Sorting & Searching Demo\n";
        std::cout << " 4. Run Routing (Dijkstra / MST / Bellman-Ford) Demo\n";
        std::cout << " 5. Run Dynamic Programming & Backtracking Demo\n";
        std::cout << " 6. Run Flow & String Matching Demo\n";
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
                runLiveSimulation();
                break;
            case 2:
                runDemonstrationScenarios();
                break;
            case 3: {
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
            case 4: {
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
            case 5: {
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
            case 6: {
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
