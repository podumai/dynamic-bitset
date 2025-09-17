message(CHECK_START "Detecting google benchmark package")

set(BITS_DYNAMIC_BITSET_BENCHMARK_INCLUDE "${CMAKE_SOURCE_DIR}/benchmark/include")
set(BENCHMARK_OUTPUT_NAME "benchmark$<$<PLATFORM_ID:Windows>:.exe>")

find_package(benchmark QUIET)

if(benchmark_FOUND)
  message(CHECK_PASS "found")
else()
  message(CHECK_FAIL "not found")
  message(STATUS "Trying to fetch and build locally")

  FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG eddb0241389718a23a42db6af5f0164b6e0139af # v1.9.4
  )
  FetchContent_MakeAvailable(benchmark)

  message(STATUS "Google benchmark has been fetched and will be built")
endif()

message(STATUS "Benchmark for bits::DynamicBitset will be built.")
set(BITS_DYNAMIC_BITSET_BENCHMARK)
add_executable(BITS_DYNAMIC_BITSET_BENCHMARK)
target_sources(
  BITS_DYNAMIC_BITSET_BENCHMARK
    PRIVATE
      FILE_SET bits_dbitset_bench_h
      TYPE HEADERS
      BASE_DIRS
        "${CMAKE_SOURCE_DIR}/dynamic-bitset/include"
        "${BITS_DYNAMIC_BITSET_BENCHMARK_INCLUDE}"
      FILES
        "${CMAKE_SOURCE_DIR}/dynamic-bitset/include/dynamic_bitset/dynamic_bitset.hpp"
        "${BITS_DYNAMIC_BITSET_BENCHMARK_INCLUDE}/dynamic_bitset/benchmark/benchmark.hpp"
    PRIVATE
      "${CMAKE_CURRENT_SOURCE_DIR}/bits/benchmark.cpp"
)
target_precompile_headers(
    BITS_DYNAMIC_BITSET_BENCHMARK
      PRIVATE
        ${CMAKE_SOURCE_DIR}/dynamic-bitset/include/dynamic_bitset/dynamic_bitset.hpp
)
target_compile_options(
    BITS_DYNAMIC_BITSET_BENCHMARK
      PRIVATE
        -DBITS_DYNAMIC_BITSET_BENCHMARK
)
target_compile_features(
    BITS_DYNAMIC_BITSET_BENCHMARK
      PRIVATE
        cxx_std_23
      PUBLIC
        cxx_std_20
)
target_link_libraries(
    BITS_DYNAMIC_BITSET_BENCHMARK
      PRIVATE
        benchmark::benchmark
)
set_target_properties(
    BITS_DYNAMIC_BITSET_BENCHMARK
      PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY
          "${CMAKE_CURRENT_BINARY_DIR}/bin/bits"
        OUTPUT_NAME
          "${BENCHMARK_OUTPUT_NAME}"
)

message(CHECK_START "Detecting boost package.")
find_package(Boost QUIET)
if(Boost_FOUND)
  message(CHECK_PASS "found")
  message(STATUS "Benchmark for boost::dynamic_bitset will be built")
  set(BOOST_DYNAMIC_BITSET_BENCHMARK)
  add_executable(BOOST_DYNAMIC_BITSET_BENCHMARK)
  target_sources(
    BOOST_DYNAMIC_BITSET_BENCHMARK
      PRIVATE
        FILE_SET boost_dbitset_bench_h
        TYPE HEADERS
        BASE_DIRS
          "${Boost_INCLUDE_DIR}"
          "${BITS_DYNAMIC_BITSET_BENCHMARK_INCLUDE}"
        FILES
          "${Boost_INCLUDE_DIR}/boost/dynamic_bitset.hpp"
          "${BITS_DYNAMIC_BITSET_BENCHMARK_INCLUDE}/dynamic_bitset/benchmark/benchmark.hpp"
      PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/boost/benchmark.cpp"
  )
  target_compile_features(
    BOOST_DYNAMIC_BITSET_BENCHMARK
      PRIVATE
        cxx_std_23
      PUBLIC
        cxx_std_20
  )
  target_link_libraries(
    BOOST_DYNAMIC_BITSET_BENCHMARK
      PRIVATE
        benchmark::benchmark
  )
  set_target_properties(
    BOOST_DYNAMIC_BITSET_BENCHMARK
      PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY
          "${CMAKE_CURRENT_BINARY_DIR}/bin/boost"
        OUTPUT_NAME
          "${BENCHMARK_OUTPUT_NAME}"
  )
else()
  message(CHECK_FAIL "not found")
  message(WARNING "Benchmark for boost::dynamic_bitset will not be built.")
endif()

message(STATUS "Benchmark for std::vector<bool> will be built")
set(STD_VECTOR_BOOL_BENCHMARK)
add_executable(STD_VECTOR_BOOL_BENCHMARK)
target_sources(
  STD_VECTOR_BOOL_BENCHMARK
    PRIVATE
      FILE_SET std_vector_bench_h
      TYPE HEADERS
      BASE_DIRS
        "${CMAKE_SOURCE_DIR}/benchmark/include"
      FILES
        "${CMAKE_SOURCE_DIR}/benchmark/include/dynamic_bitset/benchmark/benchmark.hpp"
    PRIVATE
      "${CMAKE_CURRENT_SOURCE_DIR}/std/benchmark.cpp"
)
target_compile_features(
  STD_VECTOR_BOOL_BENCHMARK
    PRIVATE
      cxx_std_23
    PUBLIC
      cxx_std_20
)
target_link_libraries(
  STD_VECTOR_BOOL_BENCHMARK
    PRIVATE
      benchmark::benchmark
)
set_target_properties(
  STD_VECTOR_BOOL_BENCHMARK
    PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY
        "${CMAKE_CURRENT_BINARY_DIR}/bin/std"
      OUTPUT_NAME
        "${BENCHMARK_OUTPUT_NAME}"
)
