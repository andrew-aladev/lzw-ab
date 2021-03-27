set (SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/code.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/header.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/max_code_bit_length.c"
)

include (DefineRelativeFilePaths)
cmake_define_relative_file_paths ("${SOURCES}")

if (LZWS_SHARED)
  set (TEST_NAME "test-invalid")
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
  set (TEST_STATIC_NAME "static-test-invalid")
  set (TEST_STATIC_TARGET "${LZWS_TARGET}-${TEST_STATIC_NAME}")

  add_executable (${TEST_STATIC_TARGET} ${SOURCES})
  target_link_libraries (${TEST_STATIC_TARGET} ${TEST_LIB_STATIC_TARGET})
  set_target_properties (
    ${TEST_STATIC_TARGET} PROPERTIES COMPILE_FLAGS
    "-D${LZWS_TARGET_UPPERCASE}_EXPORT_STATIC_FLAG"
  )

  add_test (
    NAME ${TEST_STATIC_NAME}
    COMMAND ${TEST_STATIC_TARGET}
    WORKING_DIRECTORY ${CMAKE_OUTPUT_DIRECTORY}
  )
endif ()
