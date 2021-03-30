set (SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/code.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/combination.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/random_string.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/string_and_file.c"
)

include (DefineRelativeFilePaths)
cmake_define_relative_file_paths ("${SOURCES}")

if (LZWS_SHARED)
  set (TEST_LIB_TARGET "${LZWS_TARGET}-test")

  add_library (${TEST_LIB_TARGET} SHARED ${SOURCES})
  target_link_libraries (${TEST_LIB_TARGET} ${LIB_TARGET})
  target_link_options (
    ${TEST_LIB_TARGET} PRIVATE
    "SHELL:${CMAKE_EXPORT_SHARED_LIBRARY_LD_FLAGS}"
  )
  set_target_properties (
    ${TEST_LIB_TARGET} PROPERTIES COMPILE_FLAGS
    "-D${LZWS_TARGET_UPPERCASE}_EXPORT_LIBRARY_FLAG"
  )
endif ()

if (LZWS_STATIC)
  set (TEST_LIB_STATIC_TARGET "${LZWS_TARGET}-static-test")

  add_library (${TEST_LIB_STATIC_TARGET} STATIC ${SOURCES})
  target_link_libraries (${TEST_LIB_STATIC_TARGET} ${LIB_STATIC_TARGET})
  set_target_properties (
    ${TEST_LIB_STATIC_TARGET} PROPERTIES COMPILE_FLAGS
    "-D${LZWS_TARGET_UPPERCASE}_EXPORT_STATIC_FLAG"
  )
endif ()

add_subdirectory ("valid")
add_subdirectory ("invalid")
