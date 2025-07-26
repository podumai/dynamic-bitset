message(CHECK_START "Detecting google benchmark package.")

find_package(benchmark QUIET)

if(benchmark_FOUND)
  message(CHECK_PASS "found")
  message(STATUS "Benchmark for bits::DynamicBitset will be build.")
  set(BITS_DYNAMIC_BITSET_BENCHMARK)
  add_executable(BITS_DYNAMIC_BITSET_BENCHMARK)
  target_sources(
      BITS_DYNAMIC_BITSET_BENCHMARK
        PRIVATE
          FILE_SET bits_dbitset_bench_h
          TYPE HEADERS
          BASE_DIRS
            "${CMAKE_SOURCE_DIR}/dynamic-bitset/include"
          FILES
            "${CMAKE_SOURCE_DIR}/dynamic-bitset/include/dynamic_bitset/dynamic_bitset.hpp"
        PRIVATE
          "${CMAKE_CURRENT_SOURCE_DIR}/bits_dbitset.cpp"
  )
  target_precompile_headers(
      BITS_DYNAMIC_BITSET_BENCHMARK
        PRIVATE
          ${CMAKE_SOURCE_DIR}/dynamic-bitset/include/dynamic_bitset/dynamic_bitset.hpp
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
            "${CMAKE_CURRENT_BINARY_DIR}/bits_dbitset"
  )

  message(CHECK_START "Detecting boost package.")
  find_package(Boost QUIET)
  if(Boost_FOUND)
    message(CHECK_PASS "found")
    message(STATUS "Benchmark for boost::dynamic_bitset will be build")
    set(BOOST_DYNAMIC_BITSET_BENCHMARK)
    add_executable(BOOST_DYNAMIC_BITSET_BENCHMARK)
    target_sources(
        BOOST_DYNAMIC_BITSET_BENCHMARK
          PRIVATE
            FILE_SET boost_dbitset_bench_h
            TYPE HEADERS
            BASE_DIRS
              ${Boost_INCLUDE_DIR}
            FILES
              "${Boost_INCLUDE_DIR}/boost/dynamic_bitset.hpp"
          PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/boost_dbitset.cpp"
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
              "${CMAKE_CURRENT_BINARY_DIR}/boost_dbitset"
    )
  else()
    message(CHECK_FAIL "not found")
    message(WARNING "Benchmark for boost::dynamic_bitset will not be build.")
  endif()

  message(STATUS "Benchmark for std::vector<bool> will be build")
  set(STD_VECTOR_BOOL_BENCHMARK)
  add_executable(STD_VECTOR_BOOL_BENCHMARK)
  target_sources(
      STD_VECTOR_BOOL_BENCHMARK
        PRIVATE
          "${CMAKE_CURRENT_SOURCE_DIR}/std_vector_bool.cpp"
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
            "${CMAKE_CURRENT_BINARY_DIR}/std_vector_bool"
  )
else()
  message(CHECK_FAIL "not found")
  message(WARNING "Benchmarks for bits::DynamicBitset, std::vector<bool>, boost::dynamic_bitset will not be build.")
endif()