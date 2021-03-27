set (SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.c")

include (DefineRelativeFilePaths)
cmake_define_relative_file_paths ("${SOURCES}")

if (LZWS_SHARED)
  set (CLI_TARGET "${LZWS_TARGET}-cli")
  set (CLI_NAME ${LZWS_TARGET})

  add_executable (${CLI_TARGET} ${SOURCES})
  target_link_libraries (${CLI_TARGET} ${LIB_TARGET})
  set_target_properties (
    ${CLI_TARGET} PROPERTIES OUTPUT_NAME
    ${CLI_NAME}
  )

  install (
    TARGETS ${CLI_TARGET} RUNTIME DESTINATION
    ${CMAKE_INSTALL_BINDIR}
  )
endif ()

if (LZWS_STATIC)
  set (CLI_STATIC_TARGET "${LZWS_TARGET}-static-cli")
  set (CLI_STATIC_NAME "${LZWS_TARGET}-static")

  add_executable (${CLI_STATIC_TARGET} ${SOURCES})
  target_link_libraries (${CLI_STATIC_TARGET} ${LIB_STATIC_TARGET})
  set_target_properties (
    ${CLI_STATIC_TARGET} PROPERTIES COMPILE_FLAGS
    "-D${LZWS_TARGET_UPPERCASE}_EXPORT_STATIC_FLAG"
  )
  set_target_properties (
    ${CLI_STATIC_TARGET} PROPERTIES OUTPUT_NAME
    ${CLI_STATIC_NAME}
  )

  install (
    TARGETS ${CLI_STATIC_TARGET} RUNTIME DESTINATION
    ${CMAKE_INSTALL_BINDIR}
  )
endif ()
