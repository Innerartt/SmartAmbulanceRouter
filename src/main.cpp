/**
 * @file main.cpp
 * @brief Entry point for the Smart Emergency Ambulance Routing & Hospital Allocation system.
 *
 * Author: Vivek Kumar Thakur (UPES)
 */

#include <iostream>
#include <string>
#include <vector>
#include "../include/menu.h"

static void printUsage(const char* prog) {
    std::cout << "Usage: " << prog << " [OPTIONS]\n"
              << "Options:\n"
              << "  --sim, --simulate  Run step-by-step live emergency simulation\n"
              << "  --demo             Run all 3 demonstration scenarios\n"
              << "  --benchmark        Run comprehensive algorithm benchmark suite\n"
              << "  --help             Show this help message\n"
              << "  (no args)          Launch interactive console menu\n";
}

int main(int argc, char* argv[]) {
    std::cout << "\n====================================================================\n";
    std::cout << "  🚑 SMART EMERGENCY AMBULANCE ROUTING & HOSPITAL ALLOCATION       \n";
    std::cout << "  UPES CS Project 01 | C++17 | Author: Vivek Kumar Thakur          \n";
    std::cout << "====================================================================\n";

    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--sim" || arg == "--simulate") {
            sar::runLiveSimulation();
            return 0;
        } else if (arg == "--demo") {
            sar::runDemonstrationScenarios();
            return 0;
        } else if (arg == "--benchmark") {
            sar::runDemonstrationScenarios();
            return 0;
        } else if (arg == "--help" || arg == "-h") {
            printUsage(argv[0]);
            return 0;
        } else {
            std::cout << "Unknown option: " << arg << "\n";
            printUsage(argv[0]);
            return 1;
        }
    }

    sar::runInteractiveMenu();
    return 0;
}
