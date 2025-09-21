message(STATUS "[${PROJECT_NAME}] Tests output directory:")
message(STATUS " ${CMAKE_CURRENT_BINARY_DIR}/bin")

add_executable(BITS_DYNAMIC_BITSET_TEST)
set(TEST_OUTPUT_NAME "tests$<$<PLATFORM_ID:Windows>:.exe>")
target_sources(
  BITS_DYNAMIC_BITSET_TEST
    PRIVATE
      FILE_SET bits_dynamic_bitset_test_h
      TYPE HEADERS
      BASE_DIRS
        "${CMAKE_SOURCE_DIR}/include"
      FILES
        "${CMAKE_SOURCE_DIR}/include/dynamic_bitset/dynamic_bitset.hpp"
    PRIVATE
      "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp"
)
target_precompile_headers(
  BITS_DYNAMIC_BITSET_TEST
    PRIVATE
      "${CMAKE_SOURCE_DIR}/include/dynamic_bitset/dynamic_bitset.hpp"
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
      INTERPROCEDURAL_OPTIMIZATION
        "$<${IPO_SUPPORT}:TRUE:FALSE>"
)

gtest_discover_tests(BITS_DYNAMIC_BITSET_TEST)
