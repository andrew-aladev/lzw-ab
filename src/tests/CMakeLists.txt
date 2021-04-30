set (
  SOURCES
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
    "SHELL:${CMAKE_SHARED_LIBRARY_LD_FLAGS}"
  )
  set_target_properties (
    ${TEST_LIB_TARGET} PROPERTIES
    COMPILE_FLAGS "-D${LZWS_TARGET_UPPERCASE}_EXPORT_LIBRARY_FLAG"
  )
  # MSVC provides additional static library "test_lib_target.dll.lib"
  # and pdf file "test_lib_target.dll.pdb"
  # for shared library "test_lib_target.dll".
  set_target_properties (
    ${TEST_LIB_TARGET} PROPERTIES
    ARCHIVE_OUTPUT_NAME "${TEST_LIB_TARGET}${CMAKE_SHARED_LIBRARY_SUFFIX}"
    PDB_NAME "${TEST_LIB_TARGET}${CMAKE_SHARED_LIBRARY_SUFFIX}"
  )
endif ()

if (LZWS_STATIC)
  set (TEST_LIB_STATIC_NAME "${LZWS_TARGET}-test")
  set (TEST_LIB_STATIC_TARGET "${TEST_LIB_STATIC_NAME}-static")

  add_library (${TEST_LIB_STATIC_TARGET} STATIC ${SOURCES})
  target_link_libraries (${TEST_LIB_STATIC_TARGET} ${LIB_STATIC_TARGET})
  set_target_properties (
    ${TEST_LIB_STATIC_TARGET} PROPERTIES
    COMPILE_FLAGS "-D${LZWS_TARGET_UPPERCASE}_EXPORT_STATIC_FLAG"
    OUTPUT_NAME ${TEST_LIB_STATIC_NAME}
    ARCHIVE_OUTPUT_DIRECTORY "${TEST_LIB_STATIC_TARGET}"
  )
endif ()

add_subdirectory ("valid")
add_subdirectory ("invalid")

set (TEST_TARGETS ${TEST_TARGETS} PARENT_SCOPE)
