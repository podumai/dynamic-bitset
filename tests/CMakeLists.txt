message(STATUS "[${PROJECT_NAME}] Tests output directory:")
message(STATUS " ${CMAKE_CURRENT_BINARY_DIR}/bin")

add_executable(BitsDynamicBitsetTest)
set(TEST_OUTPUT_NAME "tests$<$<PLATFORM_ID:Windows>:.exe>" CACHE INTERNAL "")
set(bits_dynamic_bitset_test_h "" CACHE INTERNAL "")
target_sources(
  BitsDynamicBitsetTest
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
  BitsDynamicBitsetTest
    PRIVATE
      "${CMAKE_SOURCE_DIR}/include/dynamic_bitset/dynamic_bitset.hpp"
)
target_compile_features(
  BitsDynamicBitsetTest
    PRIVATE
      cxx_std_20
)
target_link_libraries(
  BitsDynamicBitsetTest
    PRIVATE
      GTest::gtest
      GTest::gtest_main
)
set_target_properties(
  BitsDynamicBitsetTest
    PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY
        "${CMAKE_CURRENT_BINARY_DIR}/bin"
      OUTPUT_NAME
        "${TEST_OUTPUT_NAME}"
      INTERPROCEDURAL_OPTIMIZATION
        "$<${IPO_SUPPORT}:TRUE:FALSE>"
)

gtest_discover_tests(BitsDynamicBitsetTest)
