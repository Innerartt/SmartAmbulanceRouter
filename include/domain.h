#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace sar {

enum class Severity { CRITICAL = 1, URGENT = 2, STANDARD = 3 };

struct Location {
    double latitude;
    double longitude;

    double distanceTo(const Location& other) const;
};

struct Incident {
    int id;
    Location location;
    Severity severity;
    std::string description;
    int timestamp;          // seconds since midnight
    bool resolved;
};

struct Ambulance {
    int id;
    Location current_location;
    bool available;
    int assigned_incident_id; // -1 if free
    int capacity;             // equipment level 1-5
};

struct Hospital {
    int id;
    std::string name;
    Location location;
    int total_beds;
    int occupied_beds;
    int icu_beds;
    int icu_occupied;
    std::vector<std::string> specialties;

    int availableBeds() const noexcept;
    int availableIcuBeds() const noexcept;
};

struct Road {
    int from_node;
    int to_node;
    double distance_km;
    double travel_time_min;  // weight for routing
    double traffic_factor;   // 1.0 = normal, >1.0 = congestion

    double effectiveTravelTime() const noexcept;
};

// Conversions
std::string severityToString(Severity s);
Severity stringToSeverity(const std::string& str);

// Stream-insertion operators
std::ostream& operator<<(std::ostream& os, const Location& loc);
std::ostream& operator<<(std::ostream& os, const Incident& inc);
std::ostream& operator<<(std::ostream& os, const Ambulance& amb);
std::ostream& operator<<(std::ostream& os, const Hospital& h);
std::ostream& operator<<(std::ostream& os, const Road& r);

// Comparisons
bool operator==(const Location& a, const Location& b) noexcept;
bool operator!=(const Location& a, const Location& b) noexcept;

bool operator<(const Incident& a, const Incident& b) noexcept;
bool operator==(const Incident& a, const Incident& b) noexcept;
bool operator!=(const Incident& a, const Incident& b) noexcept;

bool operator<(const Ambulance& a, const Ambulance& b) noexcept;
bool operator==(const Ambulance& a, const Ambulance& b) noexcept;
bool operator!=(const Ambulance& a, const Ambulance& b) noexcept;

bool operator<(const Hospital& a, const Hospital& b) noexcept;
bool operator==(const Hospital& a, const Hospital& b) noexcept;
bool operator!=(const Hospital& a, const Hospital& b) noexcept;

bool operator<(const Road& a, const Road& b) noexcept;
bool operator==(const Road& a, const Road& b) noexcept;
bool operator!=(const Road& a, const Road& b) noexcept;

} // namespace sar
