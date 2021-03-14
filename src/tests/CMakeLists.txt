set (SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/code.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/combination.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/random_string.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/string_and_file.c"
)

include (DefineRelativeFilePaths)
cmake_define_relative_file_paths ("${SOURCES}")

if (LZWS_SHARED)
  set (TEST_LIB_NAME "${LZWS_TARGET}-test")
  string (TOUPPER "${LZWS_TARGET}_test" TEST_LIB_TARGET)

  if (NOT DEFINED ANY_TEST_LIB_TARGET)
    set (ANY_TEST_LIB_TARGET ${TEST_LIB_TARGET})
  endif ()

  add_library (${TEST_LIB_TARGET} SHARED ${SOURCES})
  target_link_libraries (${TEST_LIB_TARGET} ${LIB_TARGET})
  set_target_properties (${TEST_LIB_TARGET} PROPERTIES OUTPUT_NAME ${TEST_LIB_NAME})
endif ()

if (LZWS_STATIC)
  set (STATIC_TEST_LIB_NAME "${LZWS_TARGET}-static-test")
  string (TOUPPER "${LZWS_TARGET}_static_test" STATIC_TEST_LIB_TARGET)

  if (NOT DEFINED ANY_TEST_LIB_TARGET)
    set (ANY_TEST_LIB_TARGET ${STATIC_TEST_LIB_TARGET})
  endif ()

  add_library (${STATIC_TEST_LIB_TARGET} STATIC ${SOURCES})
  target_link_libraries (${STATIC_TEST_LIB_TARGET} ${STATIC_LIB_TARGET})
  set_target_properties (${STATIC_TEST_LIB_TARGET} PROPERTIES OUTPUT_NAME ${STATIC_TEST_LIB_NAME})
endif ()

# Export.
string (TOUPPER "${LZWS_TARGET}_test" TEST_LIB_BASE_NAME)

include (GenerateExportHeader)
generate_export_header (
  ${ANY_TEST_LIB_TARGET}
  BASE_NAME ${TEST_LIB_BASE_NAME}
  EXPORT_FILE_NAME "${CMAKE_CURRENT_SOURCE_DIR}/export.h"
)

if (LZWS_STATIC)
  set_target_properties(${STATIC_TEST_LIB_TARGET} PROPERTIES COMPILE_FLAGS "-D${TEST_LIB_BASE_NAME}_STATIC_DEFINE")
endif ()

# Other.
add_subdirectory ("valid")
add_subdirectory ("invalid")
