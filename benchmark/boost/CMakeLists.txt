message(CHECK_START "[${PROJECT_NAME}] Detecting boost package.")

find_package(Boost QUIET)
if(Boost_FOUND)
  message(CHECK_PASS "found")
  message(STATUS "[${PROJECT_NAME}] Benchmark for boost::dynamic_bitset will be built")

  set(BOOST_DYNAMIC_BITSET_BENCHMARK)
  set(BENCHMARK_OUTPUT_NAME "boost-benchmark$<$<PLATFORM_ID:Windows>:.exe>")
  add_executable(BOOST_DYNAMIC_BITSET_BENCHMARK)
  target_sources(
    BOOST_DYNAMIC_BITSET_BENCHMARK
      PRIVATE
        FILE_SET boost_dbitset_bench_h
        TYPE HEADERS
        BASE_DIRS
          "${Boost_INCLUDE_DIR}"
          "${CMAKE_SOURCE_DIR}/benchmark/include"
        FILES
          "${Boost_INCLUDE_DIR}/boost/dynamic_bitset.hpp"
          "${CMAKE_SOURCE_DIR}/benchmark/include/dynamic_bitset/benchmark/benchmark.hpp"
      PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/benchmark.cpp"
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
          "${CMAKE_BINARY_DIR}/benchmark/bin"
        OUTPUT_NAME
          "${BENCHMARK_OUTPUT_NAME}"
        INTERPROCEDURAL_OPTIMIZATION
          "$<${IPO_SUPPORT}:TRUE:FALSE>"
  )
else()
  message(CHECK_FAIL "not found")
  message(WARNING "[${PROJECT_NAME}] Benchmark for boost::dynamic_bitset will not be built.")
endif()
