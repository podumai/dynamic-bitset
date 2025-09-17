cmake_minimum_required(VERSION 3.27)
project(
    DYNAMIC_BITSET
    VERSION 0.1.0
    LANGUAGES CXX
    DESCRIPTION "Dynamic data structure that represents the collection of bits"
)

option(
  BUILD_TESTS
  "Build tests for ${PROJECT_NAME} if GTest package exists.
  If cmake will not find the local package then it will try to fetch and build on project level."
  ON
)
option(
  BUILD_BENCHMARKS
  "Build benchmarks for ${PROJECT_NAME} if google benchmark package exists"
  OFF
)

include(FetchContent)

if(BUILD_TESTS)
  add_subdirectory(test)
endif()

if(BUILD_BENCHMARKS)
  add_subdirectory(benchmark)
endif()
