set (SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.c")

include (DefineRelativeFilePaths)
cmake_define_relative_file_paths ("${SOURCES}")

if (LZWS_SHARED)
  set (EXAMPLE_NAME "example-string")
  set (EXAMPLE_TARGET "${LZWS_TARGET}-${EXAMPLE_NAME}")

  add_executable (${EXAMPLE_TARGET} ${SOURCES})
  target_link_libraries (${EXAMPLE_TARGET} ${LIB_TARGET})

  add_test (
    NAME ${EXAMPLE_NAME}
    COMMAND ${EXAMPLE_TARGET}
    WORKING_DIRECTORY ${CMAKE_OUTPUT_DIRECTORY}
  )
endif ()

if (LZWS_STATIC)
  set (STATIC_EXAMPLE_NAME "static-example-string")
  set (STATIC_EXAMPLE_TARGET "${LZWS_TARGET}-${STATIC_EXAMPLE_NAME}")

  add_executable (${STATIC_EXAMPLE_TARGET} ${SOURCES})
  target_link_libraries (${STATIC_EXAMPLE_TARGET} ${STATIC_LIB_TARGET})

  add_test (
    NAME ${STATIC_EXAMPLE_NAME}
    COMMAND ${STATIC_EXAMPLE_TARGET}
    WORKING_DIRECTORY ${CMAKE_OUTPUT_DIRECTORY}
  )
endif ()
