cmake_minimum_required (VERSION 3.12)
project ("cmake_check_ipo" "C")

option (CMAKE_TRY_RUN "try run target after build" OFF)

# We need to force enable IPO.
set (CMAKE_INTERPROCEDURAL_OPTIMIZATION true)

set (CHECK_NAME "check")
string (TOUPPER ${CHECK_NAME} CHECK_TARGET)

# We need to force shared library.
add_library (${CHECK_TARGET} SHARED "library.c")

set (EXECUTABLE "main")
add_executable (${EXECUTABLE} "main.c")
target_link_libraries (${EXECUTABLE} ${CHECK_TARGET})
set_target_properties (${CHECK_TARGET} PROPERTIES OUTPUT_NAME ${CHECK_NAME})

include (GenerateExportHeader)
generate_export_header (
  ${CHECK_TARGET}
  EXPORT_FILE_NAME "${CMAKE_CURRENT_SOURCE_DIR}/export.h"
)

if (CMAKE_TRY_RUN)
  add_custom_command (
    TARGET ${EXECUTABLE}
    POST_BUILD
    COMMAND ${EXECUTABLE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
endif ()
