/**
 * @file domain.cpp
 * @brief Implements all domain-model methods declared in include/domain.h.
 *
 * Part of the Smart Emergency Ambulance Routing & Hospital Allocation project.
 * Covers Haversine distance, bed availability, effective travel time,
 * severity conversions, stream-insertion operators, and comparison helpers.
 */

#include "../../include/domain.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

namespace sar {

// ---------------------------------------------------------------------------
// Constants
// ---------------------------------------------------------------------------

/// Mean radius of the Earth in kilometres (WGS-84 volumetric mean).
static constexpr double EARTH_RADIUS_KM = 6'371.0;

/// Pi – defined explicitly so the code compiles on toolchains that do not
/// expose M_PI by default (strict C++ conformance mode).
static constexpr double PI = 3.14159265358979323846;

// ---------------------------------------------------------------------------
// Helper – degree ↔ radian conversion
// ---------------------------------------------------------------------------

/**
 * @brief Convert degrees to radians.
 */
static constexpr double degToRad(double deg) noexcept {
    return deg * PI / 180.0;
}

// ---------------------------------------------------------------------------
// Location
// ---------------------------------------------------------------------------

/**
 * @brief Compute the great-circle distance to another Location using the
 *        Haversine formula.
 *
 * @param other The target location.
 * @return Distance in kilometres.
 */
double Location::distanceTo(const Location& other) const {
    const double lat1 = degToRad(latitude);
    const double lat2 = degToRad(other.latitude);
    const double dLat = degToRad(other.latitude  - latitude);
    const double dLon = degToRad(other.longitude - longitude);

    const double a = std::sin(dLat / 2.0) * std::sin(dLat / 2.0)
                   + std::cos(lat1) * std::cos(lat2)
                   * std::sin(dLon / 2.0) * std::sin(dLon / 2.0);

    const double c = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));

    return EARTH_RADIUS_KM * c;
}

// ---------------------------------------------------------------------------
// Hospital – bed queries
// ---------------------------------------------------------------------------

/**
 * @brief Return the number of general beds still available.
 */
int Hospital::availableBeds() const noexcept {
    return total_beds - occupied_beds;
}

/**
 * @brief Return the number of ICU beds still available.
 */
int Hospital::availableIcuBeds() const noexcept {
    return icu_beds - icu_occupied;
}

// ---------------------------------------------------------------------------
// Road – effective travel time
// ---------------------------------------------------------------------------

/**
 * @brief Compute the effective travel time, accounting for traffic congestion.
 *
 * @return travel_time_min × traffic_factor (in minutes).
 */
double Road::effectiveTravelTime() const noexcept {
    return travel_time_min * traffic_factor;
}

// ---------------------------------------------------------------------------
// Severity ↔ string conversions
// ---------------------------------------------------------------------------

/**
 * @brief Convert a Severity enum value to its human-readable string form.
 *
 * @param s The severity value.
 * @return "CRITICAL", "URGENT", or "STANDARD".
 */
std::string severityToString(Severity s) {
    switch (s) {
        case Severity::CRITICAL: return "CRITICAL";
        case Severity::URGENT:   return "URGENT";
        case Severity::STANDARD: return "STANDARD";
    }
    // Defensive – should never be reached with a valid enum.
    return "UNKNOWN";
}

/**
 * @brief Parse a string into a Severity value (case-insensitive).
 *
 * @param str The input string (e.g. "critical", "URGENT").
 * @return The corresponding Severity.
 * @throws std::invalid_argument if the string is not recognised.
 */
Severity stringToSeverity(const std::string& str) {
    // Work on an upper-cased copy for case-insensitive matching.
    std::string upper = str;
    std::transform(upper.begin(), upper.end(), upper.begin(),
                   [](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });

    if (upper == "CRITICAL") return Severity::CRITICAL;
    if (upper == "URGENT")   return Severity::URGENT;
    if (upper == "STANDARD") return Severity::STANDARD;

    throw std::invalid_argument("Unknown severity string: \"" + str + "\"");
}

// ---------------------------------------------------------------------------
// Stream-insertion operators
// ---------------------------------------------------------------------------

/**
 * @brief Pretty-print a Location as "(lat, lon)".
 */
std::ostream& operator<<(std::ostream& os, const Location& loc) {
    os << std::fixed << std::setprecision(6)
       << "(" << loc.latitude << ", " << loc.longitude << ")";
    return os;
}

/**
 * @brief Pretty-print an Incident with all relevant fields.
 */
std::ostream& operator<<(std::ostream& os, const Incident& inc) {
    os << "Incident #" << inc.id
       << " [" << severityToString(inc.severity) << "]"
       << " @ " << inc.location
       << " | ts=" << inc.timestamp << "s"
       << " | resolved=" << (inc.resolved ? "yes" : "no")
       << " | \"" << inc.description << "\"";
    return os;
}

/**
 * @brief Pretty-print an Ambulance with status and capacity.
 */
std::ostream& operator<<(std::ostream& os, const Ambulance& amb) {
    os << "Ambulance #" << amb.id
       << " @ " << amb.current_location
       << " | " << (amb.available ? "AVAILABLE" : "BUSY")
       << " | assigned_incident=" << amb.assigned_incident_id
       << " | capacity=" << amb.capacity;
    return os;
}

/**
 * @brief Pretty-print a Hospital with bed counts and specialties.
 */
std::ostream& operator<<(std::ostream& os, const Hospital& h) {
    os << "Hospital #" << h.id << " \"" << h.name << "\""
       << " @ " << h.location
       << " | beds=" << h.availableBeds() << "/" << h.total_beds
       << " | ICU=" << h.availableIcuBeds() << "/" << h.icu_beds
       << " | specialties=[";

    for (std::size_t i = 0; i < h.specialties.size(); ++i) {
        if (i > 0) os << ", ";
        os << h.specialties[i];
    }
    os << "]";
    return os;
}

/**
 * @brief Pretty-print a Road segment with distance and timing info.
 */
std::ostream& operator<<(std::ostream& os, const Road& r) {
    os << "Road " << r.from_node << " -> " << r.to_node
       << " | dist=" << std::fixed << std::setprecision(2)
       << r.distance_km << " km"
       << " | time=" << r.travel_time_min << " min"
       << " | traffic×" << r.traffic_factor
       << " | effective=" << r.effectiveTravelTime() << " min";
    return os;
}

// ---------------------------------------------------------------------------
// Comparison helpers
// ---------------------------------------------------------------------------

/**
 * @brief Compare two locations for exact equality.
 */
bool operator==(const Location& a, const Location& b) noexcept {
    return a.latitude == b.latitude && a.longitude == b.longitude;
}

bool operator!=(const Location& a, const Location& b) noexcept {
    return !(a == b);
}

/**
 * @brief Incidents ordered by severity (CRITICAL < URGENT < STANDARD) so that
 *        higher-priority incidents sort first, with ties broken by timestamp
 *        (earlier first) and then by id.
 */
bool operator<(const Incident& a, const Incident& b) noexcept {
    if (a.severity != b.severity)
        return static_cast<int>(a.severity) < static_cast<int>(b.severity);
    if (a.timestamp != b.timestamp)
        return a.timestamp < b.timestamp;
    return a.id < b.id;
}

bool operator==(const Incident& a, const Incident& b) noexcept {
    return a.id == b.id;
}

bool operator!=(const Incident& a, const Incident& b) noexcept {
    return !(a == b);
}

/**
 * @brief Ambulances compared by id for deterministic ordering.
 */
bool operator<(const Ambulance& a, const Ambulance& b) noexcept {
    return a.id < b.id;
}

bool operator==(const Ambulance& a, const Ambulance& b) noexcept {
    return a.id == b.id;
}

bool operator!=(const Ambulance& a, const Ambulance& b) noexcept {
    return !(a == b);
}

/**
 * @brief Hospitals compared by id for deterministic ordering.
 */
bool operator<(const Hospital& a, const Hospital& b) noexcept {
    return a.id < b.id;
}

bool operator==(const Hospital& a, const Hospital& b) noexcept {
    return a.id == b.id;
}

bool operator!=(const Hospital& a, const Hospital& b) noexcept {
    return !(a == b);
}

/**
 * @brief Roads ordered by (from_node, to_node) for adjacency-list style use.
 */
bool operator<(const Road& a, const Road& b) noexcept {
    if (a.from_node != b.from_node) return a.from_node < b.from_node;
    return a.to_node < b.to_node;
}

bool operator==(const Road& a, const Road& b) noexcept {
    return a.from_node == b.from_node && a.to_node == b.to_node;
}

bool operator!=(const Road& a, const Road& b) noexcept {
    return !(a == b);
}

} // namespace sar
