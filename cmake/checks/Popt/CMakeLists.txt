cmake_minimum_required (VERSION 3.13)
project ("cmake_check_popt" "C")

option (CMAKE_POPT_INCLUDE_PATH "Popt include path" "")
option (CMAKE_POPT_SHARED_LIBRARY_PATH "Popt shared library path" "")
option (CMAKE_POPT_STATIC_LIBRARY_PATH "Popt static library path" "")
option (CMAKE_TRY_RUN "try run target after build" OFF)

include_directories (${CMAKE_POPT_INCLUDE_PATH})

if (CMAKE_POPT_SHARED_LIBRARY_PATH)
  set (EXECUTABLE "main")
  add_executable (${EXECUTABLE} "main.c")
  target_link_libraries (${EXECUTABLE} ${CMAKE_POPT_SHARED_LIBRARY_PATH})

  if (CMAKE_TRY_RUN)
    add_custom_command (
      TARGET ${EXECUTABLE}
      POST_BUILD
      COMMAND ${EXECUTABLE}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  endif ()
endif ()

if (CMAKE_POPT_STATIC_LIBRARY_PATH)
  set (EXECUTABLE "main-static")
  add_executable (${EXECUTABLE} "main.c")
  target_link_libraries (${EXECUTABLE} ${CMAKE_POPT_STATIC_LIBRARY_PATH})

  if (CMAKE_TRY_RUN)
    add_custom_command (
      TARGET ${EXECUTABLE}
      POST_BUILD
      COMMAND ${EXECUTABLE}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  endif ()
endif ()
