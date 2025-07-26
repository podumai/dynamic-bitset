cmake_minimum_required(VERSION 3.27)
project(
    DYNAMIC_BITSET
    VERSION 1.0.1
    LANGUAGES CXX C
    DESCRIPTION "Dynamic data structure that represents the collection of bits"
)
add_subdirectory(dynamic-bitset)
add_subdirectory(test)
add_subdirectory(benchmark)