# 📖 Smart Ambulance Router — User Guide

> **Project 01** | UPES CSE | Author: Vivek Kumar Thakur ([@Innerartt](https://github.com/Innerartt))

---

## 1. Quick Start

### 1.1 Prerequisites
- A modern C++17 compiler (GCC 9+, Clang 10+, or MSVC 2019+)
- CMake 3.16+ (or Make)

### 1.2 Compilation

#### Option A: Using CMake
```bash
cd SmartAmbulanceRouter
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

#### Option B: Using Make (Linux / MinGW / WSL)
```bash
cd SmartAmbulanceRouter
make all
```

#### Option C: Direct Compilation (g++ / clang++)
```bash
cd SmartAmbulanceRouter
g++ -std=c++17 -O2 -Iinclude src/core/*.cpp src/algorithms/*.cpp src/ui/*.cpp src/main.cpp -o smart_ambulance
```

---

## 2. Command Line Modes

The compiled executable supports both direct CLI flags and an interactive console menu:

```bash
# 1. Run All 3 Demonstration Scenarios:
./smart_ambulance --demo

# 2. Run Comprehensive Benchmarks:
./smart_ambulance --benchmark

# 3. Interactive Menu Mode:
./smart_ambulance
```

---

## 3. CSV Data Format Specifications

All data files reside under the `data/` directory:

1. **Hospitals (`data/small_hospitals.csv`):**
   ```csv
   id,name,lat,lon,total_beds,occupied_beds,icu_beds,icu_occupied,specialties
   0,City General Hospital,28.6139,77.2090,120,45,20,5,Trauma;Cardiology;General
   ```

2. **Ambulances (`data/small_ambulances.csv`):**
   ```csv
   id,lat,lon,available,assigned_incident_id,capacity
   0,28.6150,77.2100,1,-1,4
   ```

3. **Roads (`data/small_roads.csv`):**
   ```csv
   from_node,to_node,distance_km,travel_time_min,traffic_factor
   0,1,3.5,7.0,1.1
   ```

4. **Incidents (`data/small_incidents.csv`):**
   ```csv
   id,lat,lon,severity,description,timestamp,resolved
   0,28.6180,77.2120,CRITICAL,Cardiac arrest at Central Market,12000,0
   ```

