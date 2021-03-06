cmake_minimum_required (VERSION 3.13)
project ("cmake_check_export_mode" "C")

option (CMAKE_EXPORT_MODE_UPPERCASE "export mode uppercase" "")
option (CMAKE_SHARED_LIBRARY_LD_FLAGS "shared library LD flags" "")
option (CMAKE_EXECUTABLE_LD_FLAGS "executable LD flags" "")
option (CMAKE_TRY_RUN "try run target after build" OFF)

configure_file ("config.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/config.h")

set (CHECK_TARGET "check")
add_library (${CHECK_TARGET} SHARED "content.c" "empty.c")
target_link_options (
  ${CHECK_TARGET} PRIVATE
  "SHELL:${CMAKE_SHARED_LIBRARY_LD_FLAGS}"
)
set_target_properties (
  ${CHECK_TARGET} PROPERTIES
  COMPILE_FLAGS "-DEXPORT_LIBRARY_FLAG"
)

set (CHECK_STATIC_TARGET "${CHECK_TARGET}_static")
add_library (${CHECK_STATIC_TARGET} STATIC "content.c" "empty.c")
set_target_properties (
  ${CHECK_STATIC_TARGET} PROPERTIES
  COMPILE_FLAGS "-DEXPORT_STATIC_FLAG"
)

set (EXECUTABLE "main")
add_executable (${EXECUTABLE} "main.c")
target_link_libraries (${EXECUTABLE} ${CHECK_TARGET})
target_link_options (
  ${EXECUTABLE} PRIVATE
  "SHELL:${CMAKE_EXECUTABLE_LD_FLAGS}"
)

set (STATIC_EXECUTABLE "${EXECUTABLE}_static")
add_executable (${STATIC_EXECUTABLE} "main.c")
target_link_libraries (${STATIC_EXECUTABLE} ${CHECK_STATIC_TARGET})
set_target_properties (
  ${STATIC_EXECUTABLE} PROPERTIES
  COMPILE_FLAGS "-DEXPORT_STATIC_FLAG"
)

if (CMAKE_TRY_RUN)
  add_custom_command (
    TARGET ${EXECUTABLE}
    POST_BUILD
    COMMAND ${EXECUTABLE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
  add_custom_command (
    TARGET ${STATIC_EXECUTABLE}
    POST_BUILD
    COMMAND ${STATIC_EXECUTABLE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
endif ()
