message(STATUS "[${PROJECT_NAME}] Benchmark for bits::DynamicBitset will be built.")

set(BITS_DYNAMIC_BITSET_BENCHMARK)
set(BENCHMARK_OUTPUT_NAME "bits-benchmark$<$<PLATFORM_ID:Windows>:.exe>")
add_executable(BITS_DYNAMIC_BITSET_BENCHMARK)
target_sources(
  BITS_DYNAMIC_BITSET_BENCHMARK
    PRIVATE
      FILE_SET bits_dbitset_bench_h
      TYPE HEADERS
      BASE_DIRS
        "${CMAKE_SOURCE_DIR}/include"
        "${CMAKE_SOURCE_DIR}/benchmark/include"
      FILES
        "${CMAKE_SOURCE_DIR}/include/dynamic_bitset/dynamic_bitset.hpp"
        "${CMAKE_SOURCE_DIR}/benchmark/include/dynamic_bitset/benchmark/benchmark.hpp"
    PRIVATE
      "${CMAKE_CURRENT_SOURCE_DIR}/benchmark.cpp"
)
target_precompile_headers(
    BITS_DYNAMIC_BITSET_BENCHMARK
      PRIVATE
        ${CMAKE_SOURCE_DIR}/include/dynamic_bitset/dynamic_bitset.hpp
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
          "${CMAKE_BINARY_DIR}/benchmark/bin"
        OUTPUT_NAME
          "${BENCHMARK_OUTPUT_NAME}"
        INTERPROCEDURAL_OPTIMIZATION
          "$<${IPO_SUPPORT}:TRUE:FALSE>"
)
