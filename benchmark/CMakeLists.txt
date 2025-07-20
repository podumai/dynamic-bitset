find_package(benchmark REQUIRED)

add_executable(${PROJECT_NAME}-BENCHMARK)
string(TOLOWER ${PROJECT_NAME}-BENCHMARK benchmark_headers)
target_sources(
    ${PROJECT_NAME}-BENCHMARK
      PRIVATE
        FILE_SET benchmark_headers
        TYPE HEADERS
        BASE_DIRS
          ${CMAKE_SOURCE_DIR}/dynamic-bitset/include
        FILES
          ${CMAKE_SOURCE_DIR}/dynamic-bitset/include/dynamic_bitset/dynamic_bitset.hpp
      PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/benchmark.cpp
)
target_precompile_headers(
    ${PROJECT_NAME}-BENCHMARK
      PRIVATE
        ${CMAKE_SOURCE_DIR}/dynamic-bitset/include/dynamic_bitset/dynamic_bitset.hpp
)
target_compile_features(
    ${PROJECT_NAME}-BENCHMARK
      PRIVATE
        cxx_std_23
      PUBLIC
        cxx_std_20
)
if(BITS_DYNAMIC_BITSET_TEST)
  set(TEST_UNIT -DBITS_DYNAMIC_BITSET_TEST CACHE STRING "Flag for bits::DynamicBitset<size_t> test")
elseif(BOOST_DYNAMIC_BITSET_TEST)
  set(TEST_UNIT -DBOOST_DYNAMIC_BITSET_TEST CACHE STRING "Flag for boost::dynamic_bitset<size_t> test")
elseif(STD_VECTOR_BOOL_TEST)
  set(TEST_UNIT -DSTD_VECTOR_BOOL_TEST CACHE STRING "Flag for std::vector<bool> test")
else()
  set(TEST_UNIT -DUNKNOWN_TEST_UNIT)
endif()
message(STATUS "Start building Google Benchmarks for ${TEST_UNIT}")
target_compile_options(
    ${PROJECT_NAME}-BENCHMARK
      PRIVATE
        ${TEST_UNIT}
)
target_link_libraries(
    ${PROJECT_NAME}-BENCHMARK
      PRIVATE
        benchmark::benchmark
)