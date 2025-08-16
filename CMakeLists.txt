cmake_minimum_required(VERSION 3.27)
project(
    DYNAMIC_BITSET
    VERSION 2.0.0
    LANGUAGES CXX
    DESCRIPTION "Dynamic data structure that represents the collection of bits"
)

option(BUILD_TESTS "Build tests for ${PROJECT_NAME} if GTest package exists" ON)
option(BUILD_BENCHMARKS "Build benchmarks if google benchmark package exists" OFF)

if(BUILD_TESTS)
  add_subdirectory(test)
endif()

if(BUILD_BENCHMARKS)
  add_subdirectory(benchmark)
endif()