add_executable(${PROJECT_NAME}_EXE)
string(TOLOWER ${PROJECT_NAME}_usage_h usage_headers)
target_sources(
    ${PROJECT_NAME}_EXE
      PRIVATE
        FILE_SET ${usage_headers}
        TYPE HEADERS
        BASE_DIRS
          ${CMAKE_CURRENT_SOURCE_DIR}/include/
        FILES
          ${CMAKE_CURRENT_SOURCE_DIR}/include/dynamic_bitset/dynamic_bitset.hpp
      PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
target_compile_features(
    ${PROJECT_NAME}_EXE
      PRIVATE
        cxx_std_23
      INTERFACE
        cxx_std_20
)