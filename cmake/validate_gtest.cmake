message(CHECK_START "[${PROJECT_NAME}] Detecting GTest package")

find_package(GTest QUIET)

if(GTest_FOUND)
  message(CHECK_PASS "found")
else()
  message(CHECK_FAIL "not found")
  message(STATUS "[${PROJECT_NAME}] Installing GTest...")

  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG 52eb8108c5bdec04579160ae17225d66034bd723 # v1.17.0
  )
  FetchContent_MakeAvailable(googletest)

  message(STATUS "[${PROJECT_NAME}] GTest has been installed")
endif()
