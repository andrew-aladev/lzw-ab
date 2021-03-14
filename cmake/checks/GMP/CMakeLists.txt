cmake_minimum_required (VERSION 3.12)
project ("cmake_check_gmp" "C")

set (CMAKE_GMP_INCLUDE_PATH        "" CACHE STRING "GMP include path")
set (CMAKE_GMP_SHARED_LIBRARY_PATH "" CACHE STRING "GMP shared library path")
set (CMAKE_GMP_STATIC_LIBRARY_PATH "" CACHE STRING "GMP static library path")

set (CMAKE_TRY_RUN false CACHE BOOL "try run targets after build")

include_directories (${CMAKE_GMP_INCLUDE_PATH})

if (CMAKE_GMP_SHARED_LIBRARY_PATH)
  add_executable ("main" "main.c")
  target_link_libraries ("main" ${CMAKE_GMP_SHARED_LIBRARY_PATH})

  if (CMAKE_TRY_RUN)
    add_custom_command (
      TARGET "main"
      POST_BUILD
      COMMAND "./main"
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  endif ()
endif ()

if (CMAKE_GMP_STATIC_LIBRARY_PATH)
  add_executable ("main-static" "main.c")
  target_link_libraries ("main-static" ${CMAKE_GMP_STATIC_LIBRARY_PATH})

  if (CMAKE_TRY_RUN)
    add_custom_command (
      TARGET "main-static"
      POST_BUILD
      COMMAND "./main-static"
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  endif ()
endif ()
