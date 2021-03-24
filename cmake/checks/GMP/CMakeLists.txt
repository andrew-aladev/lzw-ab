cmake_minimum_required (VERSION 3.12)
project ("cmake_check_gmp" "C")

set (CMAKE_GMP_INCLUDE_PATH        "" CACHE STRING "GMP include path")
set (CMAKE_GMP_SHARED_LIBRARY_PATH "" CACHE STRING "GMP shared library path")
set (CMAKE_GMP_STATIC_LIBRARY_PATH "" CACHE STRING "GMP static library path")

set (CMAKE_TRY_RUN false CACHE BOOL "try run target after build")

include_directories (${CMAKE_GMP_INCLUDE_PATH})

if (CMAKE_GMP_SHARED_LIBRARY_PATH)
  set (EXECUTABLE "main")
  add_executable (${EXECUTABLE} "main.c")
  target_link_libraries (${EXECUTABLE} ${CMAKE_GMP_SHARED_LIBRARY_PATH})

  if (CMAKE_TRY_RUN)
    add_custom_command (
      TARGET ${EXECUTABLE}
      POST_BUILD
      COMMAND ${EXECUTABLE}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  endif ()
endif ()

if (CMAKE_GMP_STATIC_LIBRARY_PATH)
  set (EXECUTABLE "main-static")
  add_executable (${EXECUTABLE} "main.c")
  target_link_libraries (${EXECUTABLE} ${CMAKE_GMP_STATIC_LIBRARY_PATH})

  if (CMAKE_TRY_RUN)
    add_custom_command (
      TARGET ${EXECUTABLE}
      POST_BUILD
      COMMAND ${EXECUTABLE}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  endif ()
endif ()
