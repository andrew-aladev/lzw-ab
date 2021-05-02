include (FetchContent)
set (FETCHCONTENT_QUIET OFF)

set (argtable3_TAG "release-v3.2.2")

if (EXISTS "${PROJECT_SOURCE_DIR}/argtable3/src")
  set (argtable3_SOURCE_DIR "${PROJECT_SOURCE_DIR}/argtable3")
elseif (LZWS_ARGTABLE3_FALLBACK STREQUAL "github")
  set (
    argtable3_FETCH_OPTIONS
    GIT_REPOSITORY "https://github.com/andrew-aladev/argtable3.git"
    GIT_TAG ${argtable3_TAG}
  )
elseif (LZWS_ARGTABLE3_FALLBACK STREQUAL "github-archive")
  set (
    argtable3_FETCH_OPTIONS
    URL "https://github.com/andrew-aladev/argtable3/archive/refs/tags/${argtable3_TAG}.tar.gz"
    URL_HASH "SHA256=4f7d6cc2b4e9722fe3c815e0c010e59da581238caa31ad77facdd309ac812032"
  )
elseif (LZWS_ARGTABLE3_FALLBACK STREQUAL "bitbucket")
  set (
    argtable3_FETCH_OPTIONS
    GIT_REPOSITORY "https://bitbucket.org/andrew-aladev/argtable3.git"
    GIT_TAG ${argtable3_TAG}
  )
elseif (LZWS_ARGTABLE3_FALLBACK STREQUAL "bitbucket-archive")
  set (
    argtable3_FETCH_OPTIONS
    URL "https://bitbucket.org/andrew-aladev/argtable3/get/${argtable3_TAG}.tar.gz"
    URL_HASH "SHA256=041d8219fe42093503d0572471bc40a972798cd36198b13525f8c7f022a6032c"
  )
elseif (LZWS_ARGTABLE3_FALLBACK STREQUAL "gitlab")
  set (
    argtable3_FETCH_OPTIONS
    GIT_REPOSITORY "https://gitlab.com/andrew-aladev/argtable3.git"
    GIT_TAG ${argtable3_TAG}
  )
elseif (LZWS_ARGTABLE3_FALLBACK STREQUAL "gitlab-archive")
  set (
    argtable3_FETCH_OPTIONS
    URL "https://gitlab.com/andrew-aladev/argtable3/-/archive/${argtable3_TAG}/argtable3-${argtable3_TAG}.tar.gz"
    URL_HASH "SHA256=30976bcd61ef9dbdc6ac78e445b79c2ac8c4be75cf846991a0a4b23dbc1909c5"
  )
else ()
  message (FATAL_ERROR "Invalid argtable3 fallback: ${LZWS_ARGTABLE3_FALLBACK}")
endif ()

if (DEFINED argtable3_FETCH_OPTIONS)
  FetchContent_Declare (argtable3 ${argtable3_FETCH_OPTIONS})
  FetchContent_GetProperties (argtable3)
  if (NOT argtable3_POPULATED)
    FetchContent_Populate (argtable3)
  endif ()
endif ()

include_directories ("${argtable3_SOURCE_DIR}/src")

set (
  SOURCES
  "${argtable3_SOURCE_DIR}/src/arg_getopt_long.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
)

include (DefineRelativeFilePaths)
cmake_define_relative_file_paths ("${SOURCES}")

if (LZWS_SHARED)
  set (CLI_NAME ${LZWS_TARGET})
  set (CLI_TARGET "${LZWS_TARGET}-cli")

  add_executable (${CLI_TARGET} ${SOURCES})
  target_compile_definitions (
    ${CLI_TARGET} PRIVATE
    "ARG_REPLACE_GETOPT=1"
  )
  target_link_libraries (${CLI_TARGET} ${LIB_TARGET})
  target_link_options (
    ${CLI_TARGET} PRIVATE
    "SHELL:${CMAKE_EXECUTABLE_LD_FLAGS}"
  )
  set_target_properties (
    ${CLI_TARGET} PROPERTIES
    OUTPUT_NAME ${CLI_NAME}
  )

  install (
    TARGETS ${CLI_TARGET} RUNTIME
    DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
endif ()

if (LZWS_STATIC)
  set (CLI_STATIC_NAME "${LZWS_TARGET}-static")
  set (CLI_STATIC_TARGET "${LZWS_TARGET}-static-cli")

  add_executable (${CLI_STATIC_TARGET} ${SOURCES})
  target_compile_definitions (
    ${CLI_STATIC_TARGET} PRIVATE
    "ARG_REPLACE_GETOPT=1"
  )
  target_link_libraries (${CLI_STATIC_TARGET} ${LIB_STATIC_TARGET})
  set_target_properties (
    ${CLI_STATIC_TARGET} PROPERTIES
    COMPILE_FLAGS "-D${LZWS_TARGET_UPPERCASE}_EXPORT_STATIC_FLAG"
    OUTPUT_NAME ${CLI_STATIC_NAME}
  )

  install (
    TARGETS ${CLI_STATIC_TARGET} RUNTIME
    DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
endif ()
