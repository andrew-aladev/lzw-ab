cmake_minimum_required (VERSION 3.12)
project ("cmake_check_tommath" "C")

set (CMAKE_TOMMATH_INCLUDE_PATH        "" CACHE STRING "TomMath include path")
set (CMAKE_TOMMATH_SHARED_LIBRARY_PATH "" CACHE STRING "TomMath shared library path")
set (CMAKE_TOMMATH_STATIC_LIBRARY_PATH "" CACHE STRING "TomMath static library path")

set (CMAKE_TRY_RUN false CACHE BOOL "try run targets after build")

include_directories (${CMAKE_TOMMATH_INCLUDE_PATH})

if (CMAKE_TOMMATH_SHARED_LIBRARY_PATH)
  add_executable ("main" "main.c")
  target_link_libraries ("main" ${CMAKE_TOMMATH_SHARED_LIBRARY_PATH})

  if (CMAKE_TRY_RUN)
    add_custom_command (
      TARGET "main"
      POST_BUILD
      COMMAND "./main"
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  endif ()
endif ()

if (CMAKE_TOMMATH_STATIC_LIBRARY_PATH)
  add_executable ("main-static" "main.c")
  target_link_libraries ("main-static" ${CMAKE_TOMMATH_STATIC_LIBRARY_PATH})

  if (CMAKE_TRY_RUN)
    add_custom_command (
      TARGET "main-static"
      POST_BUILD
      COMMAND "./main-static"
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  endif ()
endif ()
