cmake_minimum_required (VERSION 3.9)
project ("cmake_check_c18" "C")

set (CMAKE_TRY_RUN false CACHE BOOL "try run targets after build")

add_executable ("main" "main.c")

if (CMAKE_TRY_RUN)
  add_custom_command (
    TARGET "main"
    POST_BUILD
    COMMAND "./main"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
endif ()
