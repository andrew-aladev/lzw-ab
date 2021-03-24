cmake_minimum_required (VERSION 3.12)
project ("cmake_check_tommath" "C")

set (CMAKE_TOMMATH_INCLUDE_PATH        "" CACHE STRING "TomMath include path")
set (CMAKE_TOMMATH_SHARED_LIBRARY_PATH "" CACHE STRING "TomMath shared library path")
set (CMAKE_TOMMATH_STATIC_LIBRARY_PATH "" CACHE STRING "TomMath static library path")

set (CMAKE_TRY_RUN false CACHE BOOL "try run target after build")

include_directories (${CMAKE_TOMMATH_INCLUDE_PATH})

if (CMAKE_TOMMATH_SHARED_LIBRARY_PATH)
  set (EXECUTABLE "main")
  add_executable (${EXECUTABLE} "main.c")
  target_link_libraries (${EXECUTABLE} ${CMAKE_TOMMATH_SHARED_LIBRARY_PATH})

  if (CMAKE_TRY_RUN)
    add_custom_command (
      TARGET ${EXECUTABLE}
      POST_BUILD
      COMMAND ${EXECUTABLE}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  endif ()
endif ()

if (CMAKE_TOMMATH_STATIC_LIBRARY_PATH)
  set (EXECUTABLE "main-static")
  add_executable (${EXECUTABLE} "main.c")
  target_link_libraries (${EXECUTABLE} ${CMAKE_TOMMATH_STATIC_LIBRARY_PATH})

  if (CMAKE_TRY_RUN)
    add_custom_command (
      TARGET ${EXECUTABLE}
      POST_BUILD
      COMMAND ${EXECUTABLE}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  endif ()
endif ()
