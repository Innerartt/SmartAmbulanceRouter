# 🏛 System Design & Architectural Guide

> **Smart Emergency Ambulance Routing and Hospital Allocation**  
> **Course:** Design & Analysis of Algorithms | UPES CSE

---

## 1. Architectural Layers

The system follows a clean modular separation of concerns:

```
┌────────────────────────────────────────────────────────────────┐
│                    User Interface / CLI Layer                  │
│                     (src/ui/menu.cpp, main.cpp)                │
├────────────────────────────────────────────────────────────────┤
│                    Domain Dispatch Engine                      │
│                    (src/core/dispatcher.cpp)                   │
├───────────────┬────────────────┬───────────────┬───────────────┤
│    Sorting    │    Routing     │   Allocation  │  String/Text  │
│  & Searching  │    & Spanning  │    & Matching │    Analysis   │
│ (sorting.cpp) │  (greedy.cpp,  │  (dynamic_    │ (string_      │
│  (search.cpp) │   graph.cpp)   │   prog.cpp,   │   match.cpp)  │
│               │                │   flow.cpp)   │               │
├───────────────┴────────────────┴───────────────┴───────────────┤
│                       Data Layer & I/O                         │
│                    (src/core/domain.cpp,                       │
│                     src/core/persistence.cpp)                  │
└────────────────────────────────────────────────────────────────┘
```

---

## 2. Key Design Decisions

### 2.1 Graph Representation: Adjacency List vs Matrix
- **Choice:** Adjacency list using `std::unordered_map<int, std::vector<std::pair<int, double>>>` in `include/graph.h`.
- **Rationale:** Road networks in metropolitan regions are sparse graphs ($|E| \approx 2|V|$ to $4|V|$). An adjacency matrix would require $O(V^2)$ memory and slow Dijkstra from $O((V+E)\log V)$ to $O(V^2)$.
- **Dense Matrix Converter:** `Graph::getAdjacencyMatrix()` is provided specifically for Floyd-Warshall and Strassen operations when needed.

### 2.2 Template-Based Generic Algorithms
- Merge sort, quick sort, and binary search are written as modern C++17 templates in `include/sorting.h` and `include/search.h`.
- This ensures zero overhead abstraction and allows benchmark comparisons across primitive integer arrays and complex domain structs (`Incident`, `Ambulance`) using lambdas.

### 2.3 Exact vs Approximation Trade-offs
- Emergency multi-stop tours are solved using Held-Karp Bitmask DP for $N \le 20$.
- Branch-and-bound implementations provide three distinct exploration strategies:
  - **FIFO:** Breadth-first level-by-level evaluation.
  - **LIFO:** Depth-first memory-conserving search.
  - **Least-Cost (LC):** Priority queue driven best-first search using fractional knapsack bounds.

