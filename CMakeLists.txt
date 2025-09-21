cmake_minimum_required(VERSION 3.27)

set(VERSION_REGEX "#define BITS_DYNAMIC_BITSET_VERSION[ \t]+\"(.+)\"")
file(
  STRINGS
  "${CMAKE_CURRENT_SOURCE_DIR}/include/dynamic_bitset/dynamic_bitset.hpp"
  VERSION_STRING
  REGEX "${VERSION_REGEX}"
)
string(REGEX REPLACE "${VERSION_REGEX}" "\\1" VERSION_STRING "${VERSION_STRING}")

project(
    DynamicBitset
    VERSION "${VERSION_STRING}"
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
  "Build benchmarks for ${PROJECT_NAME} if google benchmark package exists.
  If cmake will not find the local package then it will try to fetch and build on project level."
  OFF
)
option(
  GENERATE_DOCS
  "Generate documentation for ${PROJECT_NAME} using Doxygen."
  OFF
)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(DEFAULT_BUILD_TYPE "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "[${PROJECT_NAME}] Setting build type to ${DEFAULT_BUILD_TYPE} as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
  set_property(
    CACHE CMAKE_BUILD_TYPE
    PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo"
  )
endif()

include(FetchContent)
include(CheckIPOSupported)
include(cmake/ccache.cmake)

set(IPO_SUPPORT "FALSE" CACHE INTERNAL "Apply interprocedural optimization for targets")
check_ipo_supported(RESULT IPO_SUPPORT)

if(BUILD_TESTS)
  enable_testing()
  include(cmake/validate_gtest.cmake)
  include(GoogleTest)
  add_subdirectory(tests)
endif()

if(BUILD_BENCHMARKS)
  include(cmake/validate_gbenchmark.cmake)
  add_subdirectory(benchmark)
endif()

if(GENERATE_DOCS)
  add_subdirectory(docs)
endif()
