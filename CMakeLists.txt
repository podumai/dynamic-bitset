cmake_minimum_required(VERSION 3.27)
project(
    DYNAMIC_BITSET
    VERSION 2.0.0
    LANGUAGES CXX
    DESCRIPTION "Dynamic data structure that represents the collection of bits"
)
add_subdirectory(dynamic-bitset)
add_subdirectory(test)
add_subdirectory(benchmark)