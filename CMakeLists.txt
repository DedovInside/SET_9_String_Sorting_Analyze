cmake_minimum_required(VERSION 3.5)

project(ContestX_skeleton CXX)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_STANDARD 20)

# the following options prevent compiler-optimization issues that are unwanted in an edu process
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

#### the list of targets ####
add_executable(StringMergeSort A1m/main.cpp)
add_executable(StringQuickSort A1q/main.cpp)
add_executable(MSD_RadixSort A1r/main.cpp)
add_executable(MSD_RadixQuickSort A1rq/main.cpp)
add_executable(Sorting_Analysis Analysing_Process/sorting_algorithms_analysis.cpp)
add_executable(StandardQuickSort StandardQuickSort/main.cpp)
add_executable(StandardMergeSort StandardMergeSort/main.cpp)