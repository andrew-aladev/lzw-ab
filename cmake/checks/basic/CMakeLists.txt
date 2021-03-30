cmake_minimum_required (VERSION 3.13)
project ("cmake_check_basic" "C")

option (CMAKE_TRY_RUN "try run target after build" OFF)

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
