cmake_minimum_required (VERSION 3.12)
project ("cmake_check_c17" "C")

set (CMAKE_TRY_RUN false CACHE BOOL "try run targets after build")

set (EXECUTABLE "main")
add_executable (${EXECUTABLE} "main.c")

if (CMAKE_TRY_RUN)
  add_custom_command (
    TARGET ${EXECUTABLE}
    POST_BUILD
    COMMAND ${EXECUTABLE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
endif ()
