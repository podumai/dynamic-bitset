message(CHECK_START "Detecting GTest package")

find_package(GTest QUIET)

if(GTest_FOUND)
  message(CHECK_PASS "found")
  message(STATUS "Tests for bits::DynamicBitset will be available in test/ directory.")
  enable_testing()

  add_executable(BITS_DYNAMIC_BITSET_TEST)
  target_sources(
      BITS_DYNAMIC_BITSET_TEST
        PRIVATE
          FILE_SET bits_dynamic_bitset_test_h
          TYPE HEADERS
          BASE_DIRS
            "${CMAKE_SOURCE_DIR}/dynamic-bitset/include"
          FILES
            "${CMAKE_SOURCE_DIR}/dynamic-bitset/include/dynamic_bitset/dynamic_bitset.hpp"
        PRIVATE
          "${CMAKE_CURRENT_SOURCE_DIR}/src/test.cpp"
  )
  target_precompile_headers(
      BITS_DYNAMIC_BITSET_TEST
        PRIVATE
          "${CMAKE_SOURCE_DIR}/dynamic-bitset/include/dynamic_bitset/dynamic_bitset.hpp"
  )
  target_compile_features(
      BITS_DYNAMIC_BITSET_TEST
        PRIVATE
          cxx_std_23
        PUBLIC
          cxx_std_20
  )
  target_link_libraries(
      BITS_DYNAMIC_BITSET_TEST
        PRIVATE
          GTest::gtest
          GTest::gtest_main
  )

  include(GoogleTest)
  gtest_discover_tests(BITS_DYNAMIC_BITSET_TEST)
else()
  message(CHECK_FAIL "not found")
  message(STATUS "Tests for bits::DynamicBitset will not be build")
endif()
