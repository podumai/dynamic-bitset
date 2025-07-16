enable_testing()
find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME}-TESTS)
string(TOLOWER ${PROJECT_NAME}-TESTS tests_headers)
target_sources(
    ${PROJECT_NAME}-TESTS
      PRIVATE
        FILE_SET tests_headers
        TYPE HEADERS
        BASE_DIRS
          ${CMAKE_SOURCE_DIR}/dynamic-bitset/include
        FILES
          ${CMAKE_SOURCE_DIR}/dynamic-bitset/include/dynamic_bitset/dynamic_bitset.hpp
      PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/test.cpp
)
target_precompile_headers(
    ${PROJECT_NAME}-TESTS
      PRIVATE
        ${CMAKE_SOURCE_DIR}/dynamic-bitset/include/dynamic_bitset/dynamic_bitset.hpp
)
target_compile_features(
    ${PROJECT_NAME}-TESTS
      PRIVATE
        cxx_std_23
      PUBLIC
        cxx_std_20
)
target_link_libraries(
    ${PROJECT_NAME}-TESTS
      PRIVATE
        GTest::gtest
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}-TESTS)