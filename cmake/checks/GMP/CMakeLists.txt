cmake_minimum_required (VERSION 3.12)
project ("cmake_check_gmp" "C")

option (CMAKE_GMP_INCLUDE_PATH "GMP include path" "")
option (CMAKE_GMP_SHARED_LIBRARY_PATH "GMP shared library path" "")
option (CMAKE_GMP_STATIC_LIBRARY_PATH "GMP static library path" "")
option (CMAKE_TRY_RUN "try run target after build" OFF)

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
