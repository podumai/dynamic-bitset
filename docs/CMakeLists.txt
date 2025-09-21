message(CHECK_START "[${PROJECT_NAME}] Detecting doxygen")

find_program(DOXYGEN_PROGRAM doxygen)
if(DOXYGEN_PROGRAM)
  message(CHECK_PASS "found")

  execute_process(
    COMMAND "${DOXYGEN_PROGRAM}" "Doxyfile"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    RESULT_VARIABLE DOXYGEN_PROGRAM_RESULT
  )

  if(NOT "${DOXYGEN_PROGRAM_RESULT}" EQUAL "0")
    message(STATUS "[${PROJECT_NAME}] Documentation output directory:")
    message(STATUS " ${CMAKE_CURRENT_SOURCE_DIR}")
  else()
    message(WARNING "[${PROJECT_NAME}] ${DOXYGEN_PROGRAM} Doxyfile failed with status code ${DOXYGEN_PROGRAM_RESULT}")
  endif()
else()
  message(CHECK_FAIL "not found")
  message(WARNING "[${PROJECT_NAME}] Skipping documenation generation process with Doxygen")
endif()
