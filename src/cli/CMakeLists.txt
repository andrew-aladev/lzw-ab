set (SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.c")

include (DefineRelativeFilePaths)
cmake_define_relative_file_paths ("${SOURCES}")

set (ARGTABLE3_TAG "release-v3.2.0.7402e6e")

if (LZWS_ARGTABLE3_MODE STREQUAL "github")
  set (
    ARGTABLE3_OPTIONS
    GIT_REPOSITORY "https://github.com/andrew-aladev/argtable3.git"
    GIT_TAG "${ARGTABLE3_TAG}"
  )
elseif (LZWS_ARGTABLE3_MODE STREQUAL "github-archive")
  set (
    ARGTABLE3_OPTIONS
    URL "https://github.com/andrew-aladev/argtable3/archive/refs/tags/${ARGTABLE3_TAG}.tar.gz"
    URL_HASH "SHA256=e4c97d1136e5656d288bed3745481beb0695190c5f20ed1aa5bb15f606f420cb"
  )
elseif (LZWS_ARGTABLE3_MODE STREQUAL "bitbucket")
  set (
    ARGTABLE3_OPTIONS
    GIT_REPOSITORY "https://bitbucket.org/andrew-aladev/argtable3.git"
    GIT_TAG "${ARGTABLE3_TAG}"
  )
elseif (LZWS_ARGTABLE3_MODE STREQUAL "bitbucket-archive")
  set (
    ARGTABLE3_OPTIONS
    URL "https://bitbucket.org/andrew-aladev/argtable3/get/${ARGTABLE3_TAG}.tar.gz"
    URL_HASH "SHA256=2635fc4d21131f9c5928bd849e1b57d67492d05eb4690348d7d36092b48217e5"
  )
elseif (LZWS_ARGTABLE3_MODE STREQUAL "gitlab")
  set (
    ARGTABLE3_OPTIONS
    GIT_REPOSITORY "https://gitlab.com/andrew-aladev/argtable3.git"
    GIT_TAG "${ARGTABLE3_TAG}"
  )
elseif (LZWS_ARGTABLE3_MODE STREQUAL "gitlab-archive")
  set (
    ARGTABLE3_OPTIONS
    URL "https://gitlab.com/andrew-aladev/argtable3/-/archive/${ARGTABLE3_TAG}/argtable3-${ARGTABLE3_TAG}.tar.gz"
    URL_HASH "SHA256=e4c97d1136e5656d288bed3745481beb0695190c5f20ed1aa5bb15f606f420cb"
  )
elseif (LZWS_ARGTABLE3_MODE STREQUAL "submodule")
  set (
    ARGTABLE3_OPTIONS
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/argtable3"
  )
else ()
  message (FATAL_ERROR "Invalid argtable3 mode: ${LZWS_ARGTABLE3_MODE}")
endif ()

include (ExternalProject)
ExternalProject_Add (
  "argtable3"
  PREFIX "argtable3"
  ${ARGTABLE3_OPTIONS}
  INSTALL_COMMAND ""
  CMAKE_ARGS
    "-DARGTABLE3_ENABLE_CONAN:BOOL=OFF"
    "-DARGTABLE3_ENABLE_TESTS:BOOL=OFF"
    "-DARGTABLE3_ENABLE_ARG_REX_DEBUG:BOOL=OFF"
    "-DARGTABLE3_BUILD_STATIC_EXAMPLES:BOOL=OFF"
    "-DARGTABLE3_REPLACE_GETOPT:BOOL=ON"
    "-DARGTABLE3_LONG_ONLY:BOOL=OFF"
)

if (LZWS_ARGTABLE3_MODE STREQUAL "submodule")
  include_directories ("${PROJECT_SOURCE_DIR}/argtable3/src")
else ()
  include_directories ("${CMAKE_CURRENT_BINARY_DIR}/argtable3/src/argtable3/src")
endif ()

link_directories ("${CMAKE_CURRENT_BINARY_DIR}/argtable3/src/argtable3-build/src")

if (LZWS_SHARED)
  set (CLI_NAME ${LZWS_TARGET})
  set (CLI_TARGET "${LZWS_TARGET}-cli")

  add_executable (${CLI_TARGET} ${SOURCES})
  add_dependencies (${CLI_TARGET} "argtable3")
  target_link_libraries (
    ${CLI_TARGET}
    ${LIB_TARGET}
    "argtable3_static"
  )
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
  add_dependencies (${CLI_STATIC_TARGET} "argtable3")
  target_link_libraries (
    ${CLI_STATIC_TARGET}
    ${LIB_STATIC_TARGET}
    "argtable3_static"
  )
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
