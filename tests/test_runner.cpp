/**
 * @file test_runner.cpp
 * @brief Comprehensive automated unit and integration test runner.
 *
 * Author: Vivek Kumar Thakur (UPES)
 */

#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <algorithm>

#include "../include/domain.h"
#include "../include/graph.h"
#include "../include/sorting.h"
#include "../include/search.h"
#include "../include/divide_conquer.h"
#include "../include/greedy.h"
#include "../include/dynamic_prog.h"
#include "../include/backtracking.h"
#include "../include/branch_bound.h"
#include "../include/flow.h"
#include "../include/number_theory.h"
#include "../include/string_match.h"
#include "../include/persistence.h"
#include "../include/dispatcher.h"

using namespace sar;

static int tests_passed = 0;
static int tests_failed = 0;

#define TEST(name) \
    std::cout << "[TEST] " << name << " ... "; \
    try {

#define END_TEST \
        std::cout << "PASSED\n"; \
        tests_passed++; \
    } catch (const std::exception& e) { \
        std::cout << "FAILED: " << e.what() << "\n"; \
        tests_failed++; \
    }

void testSorting() {
    TEST("Merge Sort & Quick Sort")
        std::vector<int> a = {9, 2, 7, 1, 5, 8, 3};
        mergeSort(a);
        assert(std::is_sorted(a.begin(), a.end()));

        std::vector<int> b = {10, -5, 33, 0, 14, 2};
        quickSort(b);
        assert(std::is_sorted(b.begin(), b.end()));
    END_TEST
}

void testSearch() {
    TEST("Binary Search Variants")
        std::vector<int> sorted = {10, 20, 30, 40, 50, 60};
        assert(binarySearchIterative(sorted, 30) == 2);
        assert(binarySearchIterative(sorted, 99) == -1);
        assert(binarySearchRecursive(sorted, 50, 0, 5) == 4);
    END_TEST
}

void testDivideConquer() {
    TEST("Divide and Conquer (Max Subarray & Fast Pow & Celebrity)")
        std::vector<int> nums = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
        auto res = maxSubarray(nums);
        assert(res.sum == 6); // [4, -1, 2, 1]

        assert(fastPower(2, 10, 1000) == 24); // 1024 % 1000 = 24

        // Celebrity: person 2 knows nobody, everyone knows 2
        std::vector<std::vector<bool>> knows = {
            {false, false, true, false},
            {false, false, true, false},
            {false, false, false, false},
            {false, true,  true, false}
        };
        assert(findCelebrity(knows, 4) == 2);
    END_TEST
}

void testGreedy() {
    TEST("Greedy Algorithms (Activity Selection, Dijkstra, MST)")
        std::vector<std::pair<int, int>> activities = {{1, 4}, {3, 5}, {0, 6}, {5, 7}, {3, 9}, {5, 9}, {6, 10}, {8, 11}};
        auto selected = activitySelection(activities);
        assert(!selected.empty());

        Graph g;
        g.addEdge(0, 1, 4.0);
        g.addEdge(0, 2, 1.0);
        g.addEdge(2, 1, 2.0);
        auto [cost, path] = dijkstraPath(g, 0, 1);
        assert(cost == 3.0); // 0 -> 2 -> 1
    END_TEST
}

void testDynamicProgramming() {
    TEST("Dynamic Programming (Knapsack, LCS, Floyd-Warshall)")
        std::vector<int> w = {2, 3, 4, 5};
        std::vector<int> v = {3, 4, 5, 6};
        auto [maxV, items] = knapsack01(w, v, 5);
        assert(maxV == 7); // item 0 (w=2,v=3) + item 1 (w=3,v=4)

        assert(longestCommonSubsequence("ABCDE", "ACE") == "ACE");
    END_TEST
}

void testBacktrackingAndBB() {
    TEST("Backtracking & Branch-and-Bound")
        auto nq4 = solveNQueens(4);
        assert(nq4.size() == 2); // 4-Queens has 2 solutions

        std::vector<int> w = {2, 3, 4, 5};
        std::vector<int> v = {3, 4, 5, 6};
        auto [vBT, itBT] = knapsackBacktracking(w, v, 5);
        auto [vLC, itLC] = knapsackLC_BB(w, v, 5);
        assert(vBT == 7);
        assert(vLC == 7);
    END_TEST
}

void testFlow() {
    TEST("Network Flow (Edmonds-Karp) & Bipartite Matching")
        FlowGraph fg(4);
        fg.addEdge(0, 1, 10);
        fg.addEdge(0, 2, 10);
        fg.addEdge(1, 3, 10);
        fg.addEdge(2, 3, 10);
        assert(fg.maxFlow(0, 3) == 20);

        std::vector<std::vector<int>> adj = {{0, 1}, {0}, {1, 2}};
        auto match = maxBipartiteMatching(adj, 3, 3);
        assert(match.size() == 3);
    END_TEST
}

void testStringAndNumberTheory() {
    TEST("String Matching & Number Theory (KMP & CRT)")
        std::string txt = "AABAACAADAABAABA";
        std::string pat = "AABA";
        auto matches = kmpSearch(txt, pat);
        assert(matches.size() == 3);
        assert(naiveStringMatch(txt, pat) == matches);
        assert(rabinKarp(txt, pat) == matches);

        // x = 2 mod 3, x = 3 mod 5, x = 2 mod 7 -> x = 23
        std::vector<long long> rem = {2, 3, 2};
        std::vector<long long> mod = {3, 5, 7};
        assert(chineseRemainderTheorem(rem, mod) == 23);
    END_TEST
}

int main() {
    std::cout << "\n====================================================================\n";
    std::cout << "  RUNNING SMART AMBULANCE ROUTER TEST SUITE                         \n";
    std::cout << "====================================================================\n";

    testSorting();
    testSearch();
    testDivideConquer();
    testGreedy();
    testDynamicProgramming();
    testBacktrackingAndBB();
    testFlow();
    testStringAndNumberTheory();

    std::cout << "\n--------------------------------------------------------------------\n";
    std::cout << "Summary: " << tests_passed << " passed, " << tests_failed << " failed.\n";
    std::cout << "====================================================================\n";

    return (tests_failed == 0) ? 0 : 1;
}

