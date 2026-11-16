# 📈 Algorithm Complexity Report

> **Project:** Smart Emergency Ambulance Routing and Hospital Allocation  
> **Course:** Design & Analysis of Algorithms | UPES CSE  
> **Author:** Vivek Kumar Thakur ([@Innerartt](https://github.com/Innerartt))

---

## 1. Asymptotic Complexity Inventory

| Module | Algorithm | Best Case | Average Case | Worst Case | Space Complexity |
|---|---|---|---|---|---|
| **Sorting** | Merge Sort | $\Theta(n \log n)$ | $\Theta(n \log n)$ | $\Theta(n \log n)$ | $O(n)$ |
| **Sorting** | Quick Sort | $\Omega(n \log n)$ | $\Theta(n \log n)$ | $O(n^2)$ | $O(\log n)$ |
| **Search** | Binary Search (Iterative) | $O(1)$ | $\Theta(\log n)$ | $O(\log n)$ | $O(1)$ |
| **Search** | Binary Search (Recursive) | $O(1)$ | $\Theta(\log n)$ | $O(\log n)$ | $O(\log n)$ |
| **D&C** | Maximum Subarray | $\Theta(n \log n)$ | $\Theta(n \log n)$ | $\Theta(n \log n)$ | $O(\log n)$ |
| **D&C** | Modular Exponentiation | $O(1)$ | $\Theta(\log \text{exp})$ | $O(\log \text{exp})$ | $O(1)$ |
| **D&C** | Celebrity Identification | $\Theta(n)$ | $\Theta(n)$ | $\Theta(n)$ | $O(1)$ |
| **D&C** | Classical Matrix Multiplication | $\Theta(n^3)$ | $\Theta(n^3)$ | $\Theta(n^3)$ | $O(n^2)$ |
| **D&C** | Strassen Matrix Multiplication | $\Theta(n^{\log_2 7}) \approx O(n^{2.807})$ | $\Theta(n^{2.807})$ | $\Theta(n^{2.807})$ | $O(n^2)$ |
| **Greedy** | Activity Selection | $O(n \log n)$ | $O(n \log n)$ | $O(n \log n)$ | $O(n)$ |
| **Greedy** | Interval Partitioning | $O(n \log n)$ | $O(n \log n)$ | $O(n \log n)$ | $O(n)$ |
| **Greedy** | Huffman Encoding | $O(n \log n)$ | $O(n \log n)$ | $O(n \log n)$ | $O(n)$ |
| **Greedy** | Dijkstra's Routing | $O((V+E)\log V)$ | $O((V+E)\log V)$ | $O((V+E)\log V)$ | $O(V)$ |
| **Greedy** | Prim's MST | $O(E \log V)$ | $O(E \log V)$ | $O(E \log V)$ | $O(V)$ |
| **Greedy** | Kruskal's MST | $O(E \log E)$ | $O(E \log E)$ | $O(E \log E)$ | $O(V)$ |
| **DP** | 0/1 Knapsack | $\Theta(n W)$ | $\Theta(n W)$ | $\Theta(n W)$ | $O(n W)$ |
| **DP** | Matrix Chain Multiplication | $\Theta(n^3)$ | $\Theta(n^3)$ | $\Theta(n^3)$ | $O(n^2)$ |
| **DP** | Longest Common Subsequence | $\Theta(m \cdot n)$ | $\Theta(m \cdot n)$ | $\Theta(m \cdot n)$ | $O(m \cdot n)$ |
| **DP** | Optimal Binary Search Tree | $\Theta(n^3)$ | $\Theta(n^3)$ | $\Theta(n^3)$ | $O(n^2)$ |
| **DP** | Bellman-Ford Shortest Path | $O(E)$ | $\Theta(V \cdot E)$ | $O(V \cdot E)$ | $O(V)$ |
| **DP** | Floyd-Warshall All-Pairs | $\Theta(V^3)$ | $\Theta(V^3)$ | $\Theta(V^3)$ | $O(V^2)$ |
| **DP** | Held-Karp TSP | $\Theta(n^2 \cdot 2^n)$ | $\Theta(n^2 \cdot 2^n)$ | $\Theta(n^2 \cdot 2^n)$ | $O(n \cdot 2^n)$ |
| **Backtracking** | N-Queens Problem | $\Omega(1)$ | Exponential | $O(n!)$ | $O(n)$ |
| **Backtracking** | Subset Sum | $\Omega(1)$ | Exponential | $O(2^n)$ | $O(n)$ |
| **B&B** | Knapsack (FIFO / LIFO / LC) | $\Omega(n \log n)$ | Exponential | $O(2^n)$ | $O(2^n)$ |
| **Flow** | Edmonds-Karp (Max Flow) | $O(V \cdot E)$ | $O(V \cdot E^2)$ | $O(V \cdot E^2)$ | $O(V + E)$ |
| **Flow** | Hopcroft-Karp Bipartite Match | $O(E \sqrt{V})$ | $O(E \sqrt{V})$ | $O(E \sqrt{V})$ | $O(V)$ |
| **Number Theory** | Chinese Remainder Theorem | $O(k \log(\text{lcm}))$ | $O(k \log M)$ | $O(k \log M)$ | $O(1)$ |
| **Strings** | Naive String Matching | $O(n)$ | $O(n \cdot m)$ | $O((n-m+1)m)$ | $O(1)$ |
| **Strings** | Rabin-Karp Search | $O(n + m)$ | $O(n + m)$ | $O(n \cdot m)$ | $O(1)$ |
| **Strings** | Finite Automaton Matching | $O(n)$ | $O(n + m\|\Sigma\|)$ | $O(n + m\|\Sigma\|)$ | $O(m\|\Sigma\|)$ |
| **Strings** | Knuth-Morris-Pratt (KMP) | $O(n + m)$ | $\Theta(n + m)$ | $\Theta(n + m)$ | $O(m)$ |

---

## 2. Recurrence Relations and Master Theorem

### 2.1 Merge Sort
$$T(n) = 2T(n/2) + \Theta(n)$$
- $a = 2, b = 2, f(n) = \Theta(n)$.
- Critical exponent: $\log_b a = \log_2 2 = 1$.
- Since $f(n) = \Theta(n^{\log_b a})$, Master Theorem **Case 2** applies:
$$T(n) = \Theta(n \log n)$$

### 2.2 Strassen's Matrix Multiplication
$$T(n) = 7T(n/2) + \Theta(n^2)$$
- $a = 7, b = 2, f(n) = \Theta(n^2)$.
- Critical exponent: $\log_2 7 \approx 2.807$.
- Since $f(n) = O(n^{\log_2 7 - \epsilon})$ for $\epsilon \approx 0.807 > 0$, Master Theorem **Case 1** applies:
$$T(n) = \Theta(n^{\log_2 7}) \approx \Theta(n^{2.81})$$

### 2.3 Binary Search
$$T(n) = T(n/2) + \Theta(1)$$
- $a = 1, b = 2, f(n) = \Theta(1)$.
- Critical exponent: $\log_2 1 = 0$.
- $f(n) = \Theta(n^0)$, Master Theorem **Case 2** applies:
$$T(n) = \Theta(\log n)$$

---

## 3. NP-Hardness and Polynomial Reduction

### Problem Definition: Optimal Ambulance Multi-Stop Routing (OAMSR)
Given a set of emergency incidents $I = \{i_1, i_2, \dots, i_k\}$ situated on a road network $G = (V, E, w)$, find a closed tour starting and ending at ambulance depot $D$ that visits each incident exactly once while minimizing the total response time $\sum \text{travel\_time}$.

### Reduction from Travelling Salesperson Problem (TSP)
1. **Decision TSP is NP-Complete:** Given graph $G' = (V', E')$ with edge weights and bound $K$, does there exist a Hamiltonian cycle of cost $\le K$?
2. **Construction:**
   - Map each vertex $v \in V'$ to an incident location $i_v$.
   - Let the distance/time between incident nodes correspond directly to the metric edge weight in $G'$.
   - An ambulance depot is placed at vertex $v_0$.
3. **Equivalence:**
   - A valid ambulance tour of cost $\le K$ exists if and only if there is a Hamiltonian tour of cost $\le K$ in $G'$.
4. **Conclusion:** Since TSP is NP-hard, OAMSR is **NP-hard**.
   - For small instances ($N \le 20$), we use the exact **Held-Karp Dynamic Programming** algorithm ($O(n^2 2^n)$) or **Least-Cost Branch & Bound**.
   - For large-scale real-time operations, greedy proximity heuristics and bipartite matching are adopted.

