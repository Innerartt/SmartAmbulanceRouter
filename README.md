# 🚑 Smart Emergency Ambulance Routing & Hospital Allocation

> **Project 01** | Duration: 8 Weeks | Language: C++17 | Author: Vivek Kumar Thakur (UPES)

A portable C++17 application that simulates a real-time emergency medical dispatch system. It computes optimal ambulance routes, allocates patients to hospitals based on capacity and severity, and benchmarks **25+ core algorithms** from every major syllabus area — sorting, divide-and-conquer, greedy, dynamic programming, backtracking, branch-and-bound, network flow, string matching, and number theory.

---

## 📋 Table of Contents

- [Project Overview](#-project-overview)
- [Architecture](#-architecture)
- [Directory Structure](#-directory-structure)
- [Syllabus Coverage Map](#-syllabus-coverage-map)
- [Algorithm Inventory](#-algorithm-inventory)
- [Domain Model](#-domain-model)
- [Demonstration Scenarios](#-demonstration-scenarios)
- [Build & Run](#-build--run)
- [Testing Strategy](#-testing-strategy)
- [Benchmark Framework](#-benchmark-framework)
- [Complexity Report](#-complexity-report)
- [Roadmap (8-Week Plan)](#-roadmap-8-week-plan)

---

## 🏥 Project Overview

### What It Does
The system models a city's emergency medical infrastructure:
- **Ambulances** are dispatched to **Incidents** (accidents, cardiac arrests, etc.)
- **Routing** uses weighted road graphs to find the fastest path
- **Hospitals** have finite bed capacity, specialty wards, and treatment constraints
- **Dispatch logic** balances response time vs. hospital load vs. patient severity

### Core Objectives
| Objective | Constraint |
|:---|:---|
| Minimize patient response time | Road distances and traffic weights |
| Respect hospital bed capacity | Each hospital has per-ward limits |
| Prioritize by emergency severity | Critical > Urgent > Standard |
| Provide overload alerts | When all nearby hospitals are full |
| Compare algorithmic strategies | Same input → 2+ algorithms → measured differences |

---

## 🏗 Architecture

```
┌──────────────────────────────────────────────────────┐
│                    CLI / Menu Interface               │
├──────────────────────────────────────────────────────┤
│              Dispatcher & Scenario Engine             │
├────────────┬────────────┬────────────┬───────────────┤
│  Sorting   │  Routing   │  Alloc.    │  Analytics    │
│  Module    │  Module    │  Module    │  Module       │
├────────────┴────────────┴────────────┴───────────────┤
│              Domain Model (Core Data Layer)           │
├──────────────────────────────────────────────────────┤
│          Persistence (CSV / JSON Import/Export)       │
└──────────────────────────────────────────────────────┘
```

**Design Principle:** Every algorithm module exposes a common interface so that algorithms can be swapped fairly for benchmarking. The domain model, algorithms, experiment runner, persistence, and UI are all in separate modules.

---

## 📁 Directory Structure

```
SmartAmbulanceRouter/
├── CMakeLists.txt              # Build configuration
├── Makefile                    # Alternative build
├── README.md                   # This file
│
├── include/                    # Public headers
│   ├── domain.h                # Ambulance, Hospital, Incident, Road structs
│   ├── graph.h                 # Weighted graph ADT
│   ├── sorting.h               # Sorting algorithm interfaces
│   ├── search.h                # Binary search variants
│   ├── divide_conquer.h        # Max subarray, Strassen, fast pow, celebrity
│   ├── greedy.h                # Activity selection, Huffman, Dijkstra, Prim, Kruskal
│   ├── dynamic_prog.h          # Knapsack, LCS, MCM, optimal BST, Bellman-Ford, Floyd, TSP
│   ├── backtracking.h          # N-Queens, subset sum, knapsack BT
│   ├── branch_bound.h          # FIFO, LIFO, LC B&B for knapsack/TSP
│   ├── flow.h                  # Ford-Fulkerson, bipartite matching
│   ├── number_theory.h         # Chinese Remainder Theorem
│   ├── string_match.h          # Naive, Rabin-Karp, FA, KMP
│   ├── dispatcher.h            # Core dispatch logic
│   └── benchmark.h             # Timing and measurement utilities
│
├── src/
│   ├── core/                   # Domain model implementations
│   │   ├── domain.cpp
│   │   ├── graph.cpp
│   │   └── persistence.cpp     # CSV/JSON I/O
│   │
│   ├── algorithms/             # All algorithm implementations
│   │   ├── sorting.cpp
│   │   ├── search.cpp
│   │   ├── divide_conquer.cpp
│   │   ├── greedy.cpp
│   │   ├── dynamic_prog.cpp
│   │   ├── backtracking.cpp
│   │   ├── branch_bound.cpp
│   │   ├── flow.cpp
│   │   ├── number_theory.cpp
│   │   └── string_match.cpp
│   │
│   ├── ui/
│   │   └── menu.cpp            # Menu-driven CLI
│   │
│   └── main.cpp                # Entry point
│
├── tests/                      # Automated test suite
│   ├── test_sorting.cpp
│   ├── test_search.cpp
│   ├── test_graph.cpp
│   ├── test_greedy.cpp
│   ├── test_dp.cpp
│   ├── test_backtracking.cpp
│   ├── test_flow.cpp
│   ├── test_string_match.cpp
│   └── test_integration.cpp
│
├── data/                       # Sample datasets
│   ├── small_city.csv          # 5 hospitals, 10 roads, 3 ambulances
│   ├── medium_city.csv         # 20 hospitals, 80 roads, 15 ambulances
│   ├── large_city.csv          # 100 hospitals, 500 roads, 50 ambulances
│   └── adversarial.csv         # Edge cases: disconnected nodes, zero capacity
│
├── benchmarks/                 # Benchmark results & scripts
│   ├── run_benchmarks.cpp
│   └── results/
│
└── docs/
    ├── design_guide.md         # Architecture & design decisions
    ├── complexity_report.md    # Recurrences, asymptotic analysis, empirical data
    ├── user_guide.md           # How to use the application
    └── demo_script.md          # Step-by-step demonstration walkthrough
```

---

## 📐 Syllabus Coverage Map

| Syllabus Area | Domain Mapping in Project | Algorithms Used |
|:---|:---|:---|
| **Foundations & Analysis** | Every module defines I/O, constraints, invariants; operation counts collected | O, Ω, Θ notation for all |
| **Recurrences** | Merge sort, quick sort, Strassen, binary search recurrences | Iteration, substitution, recursion tree, Master Theorem |
| **Divide & Conquer** | Sort ambulances by priority; search hospital by ID; Strassen for adjacency matrix ops | Merge sort, quick sort, binary search (iterative & recursive), max subarray, fast power, celebrity, Strassen |
| **Greedy** | Schedule ambulance shifts; compress dispatch logs; find shortest routes; build road MST | Activity selection, interval partitioning, Huffman, Dijkstra, Prim, Kruskal |
| **Dynamic Programming** | Allocate equipment (knapsack); compare route costs; match patient records; build optimal lookup trees | 0/1 knapsack, MCM, LCS, optimal BST, Bellman-Ford, Floyd-Warshall, bounded TSP |
| **Backtracking & B&B** | Validate ambulance zone assignments (N-Queens analog); exact allocation decisions | N-Queens, subset sum, 0/1 knapsack (BT), FIFO/LIFO/LC B&B for knapsack & TSP |
| **Flow & Matching** | Model hospital bed capacity as flow network; match ambulances to hospitals | Ford-Fulkerson, max bipartite matching |
| **Number Theory** | Distributed ambulance counter synchronization via CRT | Chinese Remainder Theorem |
| **String Matching** | Search dispatch logs, patient IDs, incident codes | Naive, Rabin-Karp, finite automaton, KMP |
| **Complexity Classes** | Classify ambulance routing (TSP) as NP-hard; reduction argument | P, NP, NP-complete, NP-hard classification; polynomial reduction |

---

## 🧮 Algorithm Inventory (25+ Implementations)

### Sorting & Searching
| # | Algorithm | Domain Use | Time Complexity |
|---|:---|:---|:---|
| 1 | **Merge Sort** | Sort incidents by severity/timestamp | O(n log n) |
| 2 | **Quick Sort** | Sort ambulances by distance to incident | O(n log n) avg |
| 3 | **Binary Search (Iterative)** | Find hospital by capacity threshold | O(log n) |
| 4 | **Binary Search (Recursive)** | Find ambulance by ID in sorted fleet | O(log n) |

### Divide & Conquer
| # | Algorithm | Domain Use | Time Complexity |
|---|:---|:---|:---|
| 5 | **Maximum Subarray** | Find peak-demand time window in dispatch history | O(n log n) |
| 6 | **Fast Exponentiation** | Compute growth projections for capacity planning | O(log n) |
| 7 | **Celebrity Identification** | Find the most-connected hospital hub node | O(n) |
| 8 | **Classical Matrix Multiplication** | Adjacency matrix operations | O(n³) |
| 9 | **Strassen Matrix Multiplication** | Optimized adjacency matrix operations | O(n^2.81) |

### Greedy
| # | Algorithm | Domain Use | Time Complexity |
|---|:---|:---|:---|
| 10 | **Activity Selection** | Schedule non-overlapping ambulance shifts | O(n log n) |
| 11 | **Interval Partitioning** | Minimize ambulances needed for overlapping shifts | O(n log n) |
| 12 | **Huffman Coding** | Compress dispatch log data | O(n log n) |
| 13 | **Dijkstra's Algorithm** | Shortest path routing (non-negative weights) | O((V+E) log V) |
| 14 | **Prim's Algorithm** | Minimum spanning tree for road infrastructure | O(E log V) |
| 15 | **Kruskal's Algorithm** | Alternative MST for road network | O(E log E) |

### Dynamic Programming
| # | Algorithm | Domain Use | Time Complexity |
|---|:---|:---|:---|
| 16 | **0/1 Knapsack** | Allocate limited medical equipment to ambulances | O(nW) |
| 17 | **Matrix Chain Multiplication** | Optimize sequence of hospital-to-hospital transfers | O(n³) |
| 18 | **Longest Common Subsequence** | Match patient medical records across hospitals | O(mn) |
| 19 | **Optimal BST** | Build fast-lookup tree for frequently queried hospitals | O(n³) |
| 20 | **Bellman-Ford** | Shortest paths with possible negative-weight edges | O(VE) |
| 21 | **Floyd-Warshall** | All-pairs shortest paths for pre-computation | O(V³) |
| 22 | **TSP (DP, bounded)** | Optimal multi-stop ambulance tour (small instances) | O(n² · 2ⁿ) |

### Backtracking & Branch-and-Bound
| # | Algorithm | Domain Use | Time Complexity |
|---|:---|:---|:---|
| 23 | **N-Queens** | Validate non-conflicting ambulance zone assignments | Exponential |
| 24 | **Subset Sum** | Check if subset of hospitals can meet demand exactly | Exponential |
| 25 | **0/1 Knapsack (Backtracking)** | Equipment allocation with pruning | Exponential |
| 26 | **FIFO Branch & Bound** | Exact knapsack/TSP decision | Exponential |
| 27 | **LIFO Branch & Bound** | Depth-first exact knapsack/TSP | Exponential |
| 28 | **Least-Cost B&B** | Best-first exact knapsack/TSP | Exponential |

### Flow & Matching
| # | Algorithm | Domain Use | Time Complexity |
|---|:---|:---|:---|
| 29 | **Ford-Fulkerson** | Model hospital bed capacity as max-flow network | O(VE²) |
| 30 | **Max Bipartite Matching** | Assign ambulances ↔ hospitals optimally | O(V · E) |

### Number Theory & String Matching
| # | Algorithm | Domain Use | Time Complexity |
|---|:---|:---|:---|
| 31 | **Chinese Remainder Theorem** | Distributed ambulance counter synchronization | O(n log n) |
| 32 | **Naive String Matching** | Search dispatch logs | O(nm) |
| 33 | **Rabin-Karp** | Fast approximate log search with hashing | O(n+m) avg |
| 34 | **Finite Automaton Matching** | Pattern match on incident event streams | O(n + m|Σ|) |
| 35 | **KMP** | Efficient pattern matching on patient IDs | O(n + m) |

---

## 🗂 Domain Model

### Core Data Structures

```cpp
// Incident severity levels
enum class Severity { CRITICAL = 1, URGENT = 2, STANDARD = 3 };

// Geographic coordinate
struct Location {
    double latitude;
    double longitude;
};

// An emergency incident
struct Incident {
    int id;
    Location location;
    Severity severity;
    std::string description;
    int timestamp;          // seconds since midnight
    bool resolved;
};

// An ambulance unit
struct Ambulance {
    int id;
    Location current_location;
    bool available;
    int assigned_incident_id; // -1 if free
    int capacity;             // equipment level 1-5
};

// A hospital
struct Hospital {
    int id;
    std::string name;
    Location location;
    int total_beds;
    int occupied_beds;
    int icu_beds;
    int icu_occupied;
    std::vector<std::string> specialties;
};

// A road segment (weighted graph edge)
struct Road {
    int from_node;
    int to_node;
    double distance_km;
    double travel_time_min;  // weight for routing
    double traffic_factor;   // 1.0 = normal, >1.0 = congestion
};
```

---

## 🎯 Demonstration Scenarios

### Scenario 1: Normal Operation
- **Input:** 5 hospitals, 10 ambulances, 8 incidents (mixed severity)
- **Algorithms:** Dijkstra routing, greedy dispatch by proximity, Ford-Fulkerson for bed allocation
- **Expected:** All patients allocated within capacity limits, response times < 15 min

### Scenario 2: Capacity Shock (Mass Casualty Event)
- **Input:** Same city, but 25 simultaneous CRITICAL incidents flood the system
- **Algorithms:** Compare Dijkstra vs. Bellman-Ford vs. Floyd-Warshall pre-computed paths; 0/1 knapsack for equipment triage; bipartite matching for reassignment
- **Expected:** Overload alerts triggered, patients rerouted to distant hospitals, degraded but functional service

### Scenario 3: Large-Data Stress Test
- **Input:** 100 hospitals, 500 roads, 50 ambulances, 200 incidents
- **Algorithms:** Benchmark merge sort vs. quick sort on incident lists; compare Prim vs. Kruskal on full road graph; stress-test TSP on bounded subgraphs
- **Expected:** Performance tables showing time, memory, and solution quality at scale

---

## ⚙ Build & Run

### Prerequisites
- C++17 compatible compiler (GCC 9+, Clang 10+, MSVC 2019+)
- CMake 3.16+ (or GNU Make)

### Build with CMake
```bash
cd SmartAmbulanceRouter
mkdir build && cd build
cmake ..
cmake --build .
```

### Build with Make
```bash
cd SmartAmbulanceRouter
make all
```

### Run
```bash
./smart_ambulance             # Launch menu-driven CLI
./smart_ambulance --demo      # Run all 3 demonstration scenarios
./smart_ambulance --benchmark # Run full benchmark suite
./smart_ambulance --test      # Run automated test suite
```

---

## 🧪 Testing Strategy

| Test Type | Description | Location |
|:---|:---|:---|
| **Unit Tests** | Test each algorithm with small, hand-verified inputs | `tests/test_*.cpp` |
| **Integration Tests** | Test dispatcher end-to-end with known scenarios | `tests/test_integration.cpp` |
| **Edge Cases** | Empty graphs, zero capacity, single node, disconnected components | Embedded in unit tests |
| **Invalid Inputs** | Negative distances, malformed CSV, out-of-range IDs | Input validation tests |
| **Adversarial Data** | Already-sorted arrays, worst-case pivot selection, dense graphs | `data/adversarial.csv` |
| **Regression Data** | Fixed golden outputs for reproducibility | `tests/golden/` |

---

## 📊 Benchmark Framework

The benchmark runner measures each algorithm on 4 dataset sizes:

| Dataset | Hospitals | Roads | Ambulances | Incidents |
|:---|:---:|:---:|:---:|:---:|
| **Small** | 5 | 10 | 3 | 8 |
| **Medium** | 20 | 80 | 15 | 40 |
| **Large** | 100 | 500 | 50 | 200 |
| **Adversarial** | 100 | 500 | 50 | 200 (worst-case patterns) |

**Metrics collected:**
- Execution time (μs, averaged over 10 runs)
- Peak memory usage (KB)
- Solution quality (total response time, patients served)
- Scalability ratio (time at N vs. time at 2N)

---

## 📈 Complexity Report

*Full report in [`docs/complexity_report.md`](docs/complexity_report.md)*

### Recurrence Examples

| Algorithm | Recurrence | Solution | Method |
|:---|:---|:---|:---|
| Merge Sort | T(n) = 2T(n/2) + O(n) | Θ(n log n) | Master Theorem (Case 2) |
| Quick Sort (avg) | T(n) = 2T(n/2) + O(n) | Θ(n log n) | Recursion Tree |
| Binary Search | T(n) = T(n/2) + O(1) | Θ(log n) | Master Theorem (Case 2) |
| Strassen | T(n) = 7T(n/2) + O(n²) | Θ(n^2.81) | Master Theorem (Case 1) |
| TSP (DP) | — | O(n² · 2ⁿ) | Direct analysis |

### NP-Hardness Argument
The ambulance routing optimization (visit all incidents, return to base, minimise total time) reduces to the **Travelling Salesperson Problem (TSP)**. TSP is NP-hard. We provide a polynomial-time reduction from the decision version of TSP to our routing problem, confirming that no polynomial-time exact algorithm is expected.

---

## 📅 Roadmap (8-Week Plan)

| Week | Deliverables |
|:---|:---|
| **Week 1** | Domain model structs, graph ADT, CSV/JSON parser, project skeleton, Git repo |
| **Week 2** | Sorting (merge, quick), binary search (iterative + recursive), unit tests |
| **Week 3** | Divide & conquer (max subarray, fast pow, celebrity, Strassen), recurrence analysis |
| **Week 4** | Greedy algorithms (activity selection, Huffman, Dijkstra, Prim, Kruskal), dispatcher v1 |
| **Week 5** | Dynamic programming (knapsack, MCM, LCS, optimal BST, Bellman-Ford, Floyd, TSP) |
| **Week 6** | Backtracking (N-Queens, subset sum) + Branch & Bound (FIFO, LIFO, LC), flow (Ford-Fulkerson, bipartite matching) |
| **Week 7** | CRT utility, string matching (Naive, Rabin-Karp, FA, KMP), complexity class analysis, benchmark runner |
| **Week 8** | Integration testing, 3 demo scenarios, benchmark report, complexity report, documentation, release tag |

---

## 👤 Author

**Vivek Kumar Thakur**
Department of Computer Science & Engineering
University of Petroleum and Energy Studies (UPES)
GitHub: [@Innerartt](https://github.com/Innerartt)

---

## 📄 License

Academic project — UPES CSE Department. Not for commercial use.

