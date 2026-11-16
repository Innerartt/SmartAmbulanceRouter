cmake_minimum_required(VERSION 3.16)
project(SmartAmbulanceRouter VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /EHsc)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

include_directories(include)

# Gather all source files
file(GLOB_RECURSE SOURCES
    "src/core/*.cpp"
    "src/algorithms/*.cpp"
    "src/ui/*.cpp"
    "src/main.cpp"
)

add_executable(smart_ambulance ${SOURCES})

# Test suite executable
file(GLOB_RECURSE TEST_SOURCES
    "src/core/*.cpp"
    "src/algorithms/*.cpp"
    "tests/test_runner.cpp"
)

add_executable(run_tests ${TEST_SOURCES})

# Benchmarks executable
file(GLOB_RECURSE BENCHMARK_SOURCES
    "src/core/*.cpp"
    "src/algorithms/*.cpp"
    "benchmarks/run_benchmarks.cpp"
)

add_executable(run_benchmarks ${BENCHMARK_SOURCES})

