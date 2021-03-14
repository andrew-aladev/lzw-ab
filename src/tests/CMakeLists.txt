set (SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/code.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/combination.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/random_string.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/string_and_file.c"
)

include (DefineRelativeFilePaths)
cmake_define_relative_file_paths ("${SOURCES}")

include (GenerateExportHeader)

string (TOUPPER "${LZWS_TARGET}_test" TEST_LIB_BASE_NAME)

if (LZWS_SHARED)
  set (TEST_LIB_NAME "${LZWS_TARGET}-test")
  string (TOUPPER "${LZWS_TARGET}_test" TEST_LIB_TARGET)

  add_library (${TEST_LIB_TARGET} SHARED ${SOURCES})
  target_link_libraries (${TEST_LIB_TARGET} ${LIB_TARGET})
  set_target_properties (${TEST_LIB_TARGET} PROPERTIES OUTPUT_NAME ${TEST_LIB_NAME})

  generate_export_header (
    ${TEST_LIB_TARGET}
    BASE_NAME ${TEST_LIB_BASE_NAME}
    EXPORT_FILE_NAME "${CMAKE_CURRENT_SOURCE_DIR}/export.h"
  )
endif ()

if (LZWS_STATIC)
  set (STATIC_TEST_LIB_NAME "${LZWS_TARGET}-static-test")
  string (TOUPPER "${LZWS_TARGET}_static_test" STATIC_TEST_LIB_TARGET)

  add_library (${STATIC_TEST_LIB_TARGET} STATIC ${SOURCES})
  target_link_libraries (${STATIC_TEST_LIB_TARGET} ${STATIC_LIB_TARGET})
  set_target_properties (${STATIC_TEST_LIB_TARGET} PROPERTIES OUTPUT_NAME ${STATIC_TEST_LIB_NAME})

  generate_export_header (
    ${STATIC_TEST_LIB_TARGET}
    BASE_NAME ${TEST_LIB_BASE_NAME}
    EXPORT_FILE_NAME "${CMAKE_CURRENT_SOURCE_DIR}/export.h"
  )
endif ()

add_subdirectory ("valid")
add_subdirectory ("invalid")
