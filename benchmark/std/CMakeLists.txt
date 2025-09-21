message(STATUS "[${PROJECT_NAME}] Benchmark for std::vector<bool> will be built")

set(STD_VECTOR_BOOL_BENCHMARK)
set(BENCHMARK_OUTPUT_NAME "std-benchmark$<$<PLATFORM_ID:Windows>:.exe>")
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
      "${CMAKE_CURRENT_SOURCE_DIR}/benchmark.cpp"
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
        "${CMAKE_BINARY_DIR}/benchmark/bin"
      OUTPUT_NAME
        "${BENCHMARK_OUTPUT_NAME}"
      INTERPROCEDURAL_OPTIMIZATION
        "$<${IPO_SUPPORT}:TRUE:FALSE>"
)
