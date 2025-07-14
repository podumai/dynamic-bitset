add_executable(${PROJECT_NAME}-EXE)
string(TOLOWER ${PROJECT_NAME}-EXE project_headers)
target_sources(
    ${PROJECT_NAME}-EXE
      PRIVATE
        FILE_SET project_headers
        TYPE HEADERS
        BASE_DIRS
          ${CMAKE_CURRENT_SOURCE_DIR}/include/
        FILES
          ${CMAKE_CURRENT_SOURCE_DIR}/include/dynamic_bitset/dynamic_bitset.hpp
      PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
target_compile_features(
    ${PROJECT_NAME}-EXE
      PRIVATE
        cxx_std_23
      INTERFACE
        cxx_std_20
)