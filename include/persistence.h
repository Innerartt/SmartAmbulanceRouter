#pragma once

/// @file persistence.h
/// @brief CSV / JSON persistence utilities for the Smart Ambulance Router.

#include <string>
#include <vector>

#include "domain.h"

namespace sar {

/// @brief Parse a single CSV line into a vector of field strings.
/// @details Handles basic double-quote escaping: a field wrapped in
///          double-quotes may contain commas and escaped quotes ("").
/// @param line  Raw text line (without trailing newline).
/// @return Vector of unquoted field values.
std::vector<std::string> parseCsvLine(const std::string& line);

/// @brief Load hospitals from a CSV file.
/// @param csvPath  Path to the CSV file.
///        Expected columns:
///        id,name,lat,lon,total_beds,occupied_beds,icu_beds,icu_occupied,specialties
///        (specialties are semicolon-separated within the field).
/// @return Vector of Hospital structs parsed from the file.
std::vector<Hospital> loadHospitals(const std::string& csvPath);

/// @brief Load ambulances from a CSV file.
/// @param csvPath  Path to the CSV file.
///        Expected columns:
///        id,lat,lon,available(0/1),assigned_incident_id,capacity
/// @return Vector of Ambulance structs parsed from the file.
std::vector<Ambulance> loadAmbulances(const std::string& csvPath);

/// @brief Load roads (graph edges) from a CSV file.
/// @param csvPath  Path to the CSV file.
///        Expected columns:
///        from_node,to_node,distance_km,travel_time_min,traffic_factor
/// @return Vector of Road structs parsed from the file.
std::vector<Road> loadRoads(const std::string& csvPath);

/// @brief Load incidents from a CSV file.
/// @param csvPath  Path to the CSV file.
///        Expected columns:
///        id,lat,lon,severity(CRITICAL/URGENT/STANDARD),description,timestamp,resolved(0/1)
/// @return Vector of Incident structs parsed from the file.
std::vector<Incident> loadIncidents(const std::string& csvPath);

/// @brief Write a JSON-formatted result string to a file.
/// @param path         Output file path.
/// @param jsonContent  The JSON string to persist.
void saveResults(const std::string& path, const std::string& jsonContent);

} // namespace sar
