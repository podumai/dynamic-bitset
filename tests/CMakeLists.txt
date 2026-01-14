message(STATUS "[${PROJECT_NAME}] Tests output directory:")
message(STATUS " ${CMAKE_CURRENT_BINARY_DIR}/bin")

add_executable(BitsDynamicBitsetTest)
target_sources(
  BitsDynamicBitsetTest
  PRIVATE
  FILE_SET HEADERS
  BASE_DIRS "${CMAKE_SOURCE_DIR}/include"
  FILES "${CMAKE_SOURCE_DIR}/include/dynamic_bitset/dynamic_bitset.hpp"
  PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp"
)
target_precompile_headers(
  BitsDynamicBitsetTest
  PRIVATE
  "${CMAKE_SOURCE_DIR}/include/dynamic_bitset/dynamic_bitset.hpp"
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
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
  OUTPUT_NAME "tests"
  INTERPROCEDURAL_OPTIMIZATION "$<${IPO_SUPPORT}:TRUE:FALSE>"
)

gtest_discover_tests(BitsDynamicBitsetTest)
