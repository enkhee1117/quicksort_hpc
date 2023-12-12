cmake_minimum_required(VERSION 3.26)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled quicksort.cpp
        quicksort_openmp.cpp
        quicksort_mpi.cpp)
