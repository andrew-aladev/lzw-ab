set (SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/code.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/combination.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/random_string.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/string_and_file.c"
)

include (DefineRelativeFilePaths)
cmake_define_relative_file_paths ("${SOURCES}")

include (GenerateExportHeader)

if (LZWS_SHARED)
  set (TEST_LIB_TARGET "${LZWS_TARGET}-test")
  string (TOUPPER "${LZWS_TARGET}_test" TEST_LIB_TARGET_MACRO)

  add_library (${TEST_LIB_TARGET} SHARED ${SOURCES})
  target_link_libraries (${TEST_LIB_TARGET} ${LIB_TARGET})

  generate_export_header (
    ${TEST_LIB_TARGET}
    BASE_NAME ${TEST_LIB_TARGET_MACRO}
    EXPORT_MACRO_NAME "${TEST_LIB_TARGET_MACRO}_EXPORT"
    EXPORT_FILE_NAME "${CMAKE_CURRENT_SOURCE_DIR}/export.h"
  )
endif ()

if (LZWS_STATIC)
  set (STATIC_TEST_LIB_TARGET "${LZWS_TARGET}-static-test")
  string (TOUPPER "${LZWS_TARGET}_static_test" STATIC_TEST_LIB_TARGET_MACRO)

  add_library (${STATIC_TEST_LIB_TARGET} STATIC ${SOURCES})
  target_link_libraries (${STATIC_TEST_LIB_TARGET} ${STATIC_LIB_TARGET})

  generate_export_header (
    ${STATIC_TEST_LIB_TARGET}
    BASE_NAME ${STATIC_TEST_LIB_TARGET_MACRO}
    EXPORT_MACRO_NAME "${STATIC_TEST_LIB_TARGET_MACRO}_EXPORT"
    EXPORT_FILE_NAME "${CMAKE_CURRENT_SOURCE_DIR}/export.h"
  )
endif ()

add_subdirectory ("valid")
add_subdirectory ("invalid")
