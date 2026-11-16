# 🎬 Demonstration Script & Viva Walkthrough

> **Smart Emergency Ambulance Routing and Hospital Allocation**  
> **Course:** Design & Analysis of Algorithms | UPES CSE

---

## 1. Project Introduction (2 Minutes)

- **Problem:** Emergency medical response systems must minimize patient transport delays while balancing hospital bed capacity and matching medical specialties to trauma severity.
- **Solution:** A modular C++17 framework implementing 35+ standard syllabus algorithms behind domain abstractions.
- **Core Syllabus Coverage:**
  1. **Divide & Conquer:** Merge sort for incident triage, Strassen matrix multiplication, fast modular exponentiation.
  2. **Greedy:** Dijkstra shortest paths, Prim/Kruskal minimum spanning trees, interval partitioning.
  3. **Dynamic Programming:** 0/1 knapsack equipment triage, Bellman-Ford, Floyd-Warshall, Held-Karp TSP tour.
  4. **Branch & Bound / Backtracking:** FIFO/LIFO/LC bounds on combinatorial assignments, N-Queens zone verification.
  5. **Network Flow:** Edmonds-Karp capacity network modeling, maximum bipartite matching.
  6. **String Matching & Number Theory:** KMP & Rabin-Karp log search, Chinese Remainder Theorem dispatch sync.

---

## 2. Live Scenario Demonstrations

### Scenario 1: Normal City Operations
- Run: `./smart_ambulance --demo`
- **Output:** Dispatches nearest ambulances to incidents using Dijkstra routing and Haversine distance calculations. Allocates hospital beds based on general and ICU availability. Average response time under 15 minutes.

### Scenario 2: Mass Casualty Influx (Capacity Shock)
- **Simulates:** 25 simultaneous critical incidents.
- **Observation:** Nearby hospital trauma centers reach 100% capacity; automatic alert triggers and reroutes ambulances to tertiary hubs with available ICU beds.

### Scenario 3: Algorithmic Benchmarks
- **Run:** `./smart_ambulance --benchmark`
- **Observation:** Formatted speedup tables comparing Merge Sort vs Quick Sort at scale, Dijkstra vs Prim vs Kruskal, and 0/1 Knapsack DP vs Branch-and-Bound variants.

