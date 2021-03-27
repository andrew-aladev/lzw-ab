set (SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.c")

include (DefineRelativeFilePaths)
cmake_define_relative_file_paths ("${SOURCES}")

if (LZWS_SHARED)
  set (EXAMPLE_NAME "example-generic")
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
  set (EXAMPLE_STATIC_NAME "static-example-generic")
  set (EXAMPLE_STATIC_TARGET "${LZWS_TARGET}-${EXAMPLE_STATIC_NAME}")

  add_executable (${EXAMPLE_STATIC_TARGET} ${SOURCES})
  target_link_libraries (${EXAMPLE_STATIC_TARGET} ${LIB_STATIC_TARGET})
  set_target_properties (
    ${EXAMPLE_STATIC_TARGET} PROPERTIES COMPILE_FLAGS
    "-D${LZWS_TARGET_UPPERCASE}_EXPORT_STATIC_FLAG"
  )

  add_test (
    NAME ${EXAMPLE_STATIC_NAME}
    COMMAND ${EXAMPLE_STATIC_TARGET}
    WORKING_DIRECTORY ${CMAKE_OUTPUT_DIRECTORY}
  )
endif ()
