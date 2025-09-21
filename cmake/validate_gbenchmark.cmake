message(CHECK_START "[${PROJECT_NAME}] Detecting google benchmark package")

find_package(benchmark QUIET)

if(benchmark_FOUND)
  message(CHECK_PASS "found")
else()
  message(CHECK_FAIL "not found")
  message(STATUS "[${PROJECT_NAME}] Installing google benchmark...")

  FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG eddb0241389718a23a42db6af5f0164b6e0139af # v1.9.4
  )
  FetchContent_MakeAvailable(benchmark)

  message(STATUS "[${PROJECT_NAME}] Google benchmarks have been installed")
endif()
