message(CHECK_START "Detecting GTest package")

set(TEST_OUTPUT_NAME "test$<$<PLATFORM_ID:Windows>:.exe>")

find_package(GTest QUIET)

if (GTest_FOUND)
  message(CHECK_PASS "found")
else()
  message(CHECK_FAIL "not found")
  message(STATUS "Trying to fetch and build locally")
  
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG 52eb8108c5bdec04579160ae17225d66034bd723 # v1.17.0
  )
  FetchContent_MakeAvailable(googletest)

  message(STATUS "Google test benchmark has been fetched and will be built")
endif()

message(STATUS "Tests for bits::DynamicBitset will be available in ${CMAKE_CURRENT_BINARY_DIR}/bin directory.")

enable_testing()
include(GoogleTest)

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
set_target_properties(
  BITS_DYNAMIC_BITSET_TEST
    PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY
        "${CMAKE_CURRENT_BINARY_DIR}/bin"
      OUTPUT_NAME
        "${TEST_OUTPUT_NAME}"
)

gtest_discover_tests(BITS_DYNAMIC_BITSET_TEST)
