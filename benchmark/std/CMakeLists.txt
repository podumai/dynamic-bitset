message(STATUS "[${PROJECT_NAME}] Benchmark for std::vector<bool> will be built")

set(StdVectorBoolBenchmark)
add_executable(StdVectorBoolBenchmark)
target_sources(
  StdVectorBoolBenchmark
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
  StdVectorBoolBenchmark
    PRIVATE
      cxx_std_20
)
target_link_libraries(
  StdVectorBoolBenchmark
    PRIVATE
      benchmark::benchmark
)
set_target_properties(
  StdVectorBoolBenchmark
    PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY
        "${CMAKE_BINARY_DIR}/benchmark/bin"
      OUTPUT_NAME
        "std-benchmark"
      INTERPROCEDURAL_OPTIMIZATION
        "$<${IPO_SUPPORT}:TRUE:FALSE>"
)
