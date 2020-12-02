set (SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/code.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/data.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
)

include (DefineRelativeFilePaths)
cmake_define_relative_file_paths ("${SOURCES}")

if (LZWS_SHARED)
  set (TEST_NAME "test-valid")
  set (TEST_TARGET "${LZWS_TARGET}-${TEST_NAME}")

  add_executable (${TEST_TARGET} ${SOURCES})
  target_link_libraries (${TEST_TARGET} ${TEST_LIB_TARGET})

  add_test (
    NAME ${TEST_NAME}
    COMMAND ${TEST_TARGET}
    WORKING_DIRECTORY ${CMAKE_OUTPUT_DIRECTORY}
  )
endif ()

if (LZWS_STATIC)
  set (STATIC_TEST_NAME "static-test-valid")
  set (STATIC_TEST_TARGET "${LZWS_TARGET}-${STATIC_TEST_NAME}")

  add_executable (${STATIC_TEST_TARGET} ${SOURCES})
  target_link_libraries (${STATIC_TEST_TARGET} ${STATIC_TEST_LIB_TARGET})

  add_test (
    NAME ${STATIC_TEST_NAME}
    COMMAND ${STATIC_TEST_TARGET}
    WORKING_DIRECTORY ${CMAKE_OUTPUT_DIRECTORY}
  )
endif ()
