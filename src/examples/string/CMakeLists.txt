set (SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.c")

include (DefineRelativeFilePaths)
cmake_define_relative_file_paths ("${SOURCES}")

if (LZWS_SHARED)
  set (EXAMPLE_NAME "example-string")
  set (EXAMPLE_TARGET "${LZWS_TARGET}-${EXAMPLE_NAME}")

  add_executable (${EXAMPLE_TARGET} ${SOURCES})
  target_link_libraries (${EXAMPLE_TARGET} ${LIB_TARGET})
  target_link_options (
    ${EXAMPLE_TARGET} PRIVATE
    "SHELL:${CMAKE_EXECUTABLE_LD_FLAGS}"
  )

  add_test (
    NAME ${EXAMPLE_NAME}
    COMMAND ${EXAMPLE_TARGET}
    WORKING_DIRECTORY ${CMAKE_OUTPUT_DIRECTORY}
  )
  set (TEST_TARGETS ${TEST_TARGETS} ${EXAMPLE_TARGET})
endif ()

if (LZWS_STATIC)
  set (EXAMPLE_STATIC_NAME "static-example-string")
  set (EXAMPLE_STATIC_TARGET "${LZWS_TARGET}-${EXAMPLE_STATIC_NAME}")

  add_executable (${EXAMPLE_STATIC_TARGET} ${SOURCES})
  target_link_libraries (${EXAMPLE_STATIC_TARGET} ${LIB_STATIC_TARGET})
  set_target_properties (
    ${EXAMPLE_STATIC_TARGET} PROPERTIES
    COMPILE_FLAGS "-D${LZWS_TARGET_UPPERCASE}_EXPORT_STATIC_FLAG"
  )

  add_test (
    NAME ${EXAMPLE_STATIC_NAME}
    COMMAND ${EXAMPLE_STATIC_TARGET}
    WORKING_DIRECTORY ${CMAKE_OUTPUT_DIRECTORY}
  )
  set (TEST_TARGETS ${TEST_TARGETS} ${EXAMPLE_STATIC_TARGET})
endif ()

set (TEST_TARGETS ${TEST_TARGETS} PARENT_SCOPE)
