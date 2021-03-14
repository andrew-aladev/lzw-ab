cmake_minimum_required (VERSION 3.12)
project ("cmake_check_ipo" "C")

set (CMAKE_TRY_RUN false CACHE BOOL "try run targets after build")

# We need to force enable IPO.
set (CMAKE_INTERPROCEDURAL_OPTIMIZATION true)

set (CHECK_NAME "check")
string (TOUPPER ${CHECK_NAME} CHECK_TARGET)

# We need to force shared library.
add_library (${CHECK_TARGET} SHARED "library.c")

include (GenerateExportHeader)
generate_export_header (
  ${CHECK_TARGET}
  EXPORT_FILE_NAME "${CMAKE_CURRENT_SOURCE_DIR}/export.h"
)

add_executable ("main" "main.c")
target_link_libraries ("main" ${CHECK_TARGET})
set_target_properties (${CHECK_TARGET} PROPERTIES OUTPUT_NAME ${CHECK_NAME})

if (CMAKE_TRY_RUN)
  add_custom_command (
    TARGET "main"
    POST_BUILD
    COMMAND "./main"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
endif ()
