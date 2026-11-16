/**
 * @file persistence.cpp
 * @brief Implementation of CSV/JSON loading and saving for Smart Ambulance Router.
 */

#include "../../include/persistence.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>

namespace sar {

std::vector<std::string> parseCsvLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string current;
    bool in_quotes = false;

    for (size_t i = 0; i < line.size(); ++i) {
        char ch = line[i];
        if (ch == '\"') {
            if (in_quotes && i + 1 < line.size() && line[i + 1] == '\"') {
                current += '\"';
                ++i;
            } else {
                in_quotes = !in_quotes;
            }
        } else if (ch == ',' && !in_quotes) {
            fields.push_back(current);
            current.clear();
        } else {
            current += ch;
        }
    }
    fields.push_back(current);

    // Strip carriage return if present
    for (auto& f : fields) {
        if (!f.empty() && f.back() == '\r') {
            f.pop_back();
        }
    }
    return fields;
}

std::vector<Hospital> loadHospitals(const std::string& csvPath) {
    std::vector<Hospital> hospitals;
    std::ifstream file(csvPath);
    if (!file.is_open()) {
        std::cerr << "[Persistence Error] Failed to open hospitals CSV: " << csvPath << "\n";
        return hospitals;
    }

    std::string line;
    // Skip header line
    if (!std::getline(file, line)) return hospitals;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto tokens = parseCsvLine(line);
        if (tokens.size() < 8) continue;

        try {
            Hospital h;
            h.id = std::stoi(tokens[0]);
            h.name = tokens[1];
            h.location.latitude = std::stod(tokens[2]);
            h.location.longitude = std::stod(tokens[3]);
            h.total_beds = std::stoi(tokens[4]);
            h.occupied_beds = std::stoi(tokens[5]);
            h.icu_beds = std::stoi(tokens[6]);
            h.icu_occupied = std::stoi(tokens[7]);

            if (tokens.size() >= 9 && !tokens[8].empty()) {
                std::stringstream ss(tokens[8]);
                std::string spec;
                while (std::getline(ss, spec, ';')) {
                    if (!spec.empty()) h.specialties.push_back(spec);
                }
            }
            hospitals.push_back(h);
        } catch (const std::exception& e) {
            std::cerr << "[Persistence Warning] Error parsing hospital line: " << e.what() << "\n";
        }
    }
    return hospitals;
}

std::vector<Ambulance> loadAmbulances(const std::string& csvPath) {
    std::vector<Ambulance> ambulances;
    std::ifstream file(csvPath);
    if (!file.is_open()) {
        std::cerr << "[Persistence Error] Failed to open ambulances CSV: " << csvPath << "\n";
        return ambulances;
    }

    std::string line;
    if (!std::getline(file, line)) return ambulances;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto tokens = parseCsvLine(line);
        if (tokens.size() < 6) continue;

        try {
            Ambulance amb;
            amb.id = std::stoi(tokens[0]);
            amb.current_location.latitude = std::stod(tokens[1]);
            amb.current_location.longitude = std::stod(tokens[2]);
            amb.available = (std::stoi(tokens[3]) != 0);
            amb.assigned_incident_id = std::stoi(tokens[4]);
            amb.capacity = std::stoi(tokens[5]);
            ambulances.push_back(amb);
        } catch (const std::exception& e) {
            std::cerr << "[Persistence Warning] Error parsing ambulance line: " << e.what() << "\n";
        }
    }
    return ambulances;
}

std::vector<Road> loadRoads(const std::string& csvPath) {
    std::vector<Road> roads;
    std::ifstream file(csvPath);
    if (!file.is_open()) {
        std::cerr << "[Persistence Error] Failed to open roads CSV: " << csvPath << "\n";
        return roads;
    }

    std::string line;
    if (!std::getline(file, line)) return roads;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto tokens = parseCsvLine(line);
        if (tokens.size() < 5) continue;

        try {
            Road r;
            r.from_node = std::stoi(tokens[0]);
            r.to_node = std::stoi(tokens[1]);
            r.distance_km = std::stod(tokens[2]);
            r.travel_time_min = std::stod(tokens[3]);
            r.traffic_factor = std::stod(tokens[4]);
            roads.push_back(r);
        } catch (const std::exception& e) {
            std::cerr << "[Persistence Warning] Error parsing road line: " << e.what() << "\n";
        }
    }
    return roads;
}

std::vector<Incident> loadIncidents(const std::string& csvPath) {
    std::vector<Incident> incidents;
    std::ifstream file(csvPath);
    if (!file.is_open()) {
        std::cerr << "[Persistence Error] Failed to open incidents CSV: " << csvPath << "\n";
        return incidents;
    }

    std::string line;
    if (!std::getline(file, line)) return incidents;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto tokens = parseCsvLine(line);
        if (tokens.size() < 7) continue;

        try {
            Incident inc;
            inc.id = std::stoi(tokens[0]);
            inc.location.latitude = std::stod(tokens[1]);
            inc.location.longitude = std::stod(tokens[2]);
            inc.severity = stringToSeverity(tokens[3]);
            inc.description = tokens[4];
            inc.timestamp = std::stoi(tokens[5]);
            inc.resolved = (std::stoi(tokens[6]) != 0);
            incidents.push_back(inc);
        } catch (const std::exception& e) {
            std::cerr << "[Persistence Warning] Error parsing incident line: " << e.what() << "\n";
        }
    }
    return incidents;
}

void saveResults(const std::string& path, const std::string& jsonContent) {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "[Persistence Error] Failed to write results to: " << path << "\n";
        return;
    }
    file << jsonContent;
}

} // namespace sar

