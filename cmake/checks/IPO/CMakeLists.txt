cmake_minimum_required (VERSION 3.9)
project ("cmake_check_ipo" "C")

set (CMAKE_TRY_RUN false CACHE BOOL "try run targets after build")

# We need to force enable IPO.
set (CMAKE_INTERPROCEDURAL_OPTIMIZATION true)

add_library ("library" "library.c")

add_executable ("main" "main.c")
target_link_libraries ("main" "library")

if (CMAKE_TRY_RUN)
  add_custom_command (
    TARGET "main"
    POST_BUILD
    COMMAND "./main"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
endif ()
