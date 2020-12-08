cmake_minimum_required (VERSION 3.9)
project ("lzws" "C")

if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message (FATAL_ERROR "In-source builds are not allowed")
endif ()

set (LZWS_VERSION "1.4.2")

if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set (CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "output directory for libraries")
endif ()
if (NOT DEFINED CMAKE_INSTALL_INCLUDEDIR)
  set (CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "output directory for include files")
endif ()
if (NOT DEFINED CMAKE_INSTALL_BINDIR)
  set (CMAKE_INSTALL_BINDIR "bin" CACHE PATH "output directory for binaries")
endif ()
if (NOT DEFINED CMAKE_INSTALL_MANDIR)
  set (CMAKE_INSTALL_MANDIR "share/man" CACHE PATH "Output directory for man files")
endif ()

# Common output directory is required for OS without rpath support.
set (CMAKE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/result")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_OUTPUT_DIRECTORY})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_OUTPUT_DIRECTORY})
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_OUTPUT_DIRECTORY})

if (NOT DEFINED LZWS_TARGET)
  set (LZWS_TARGET ${PROJECT_NAME})
endif ()

set (LZWS_COMPRESSOR_DICTIONARY "sparse-array" CACHE STRING "dictionary implementation")

option (LZWS_SHARED   "build shared binaries" ON)
option (LZWS_STATIC   "build static binaries" OFF)
option (LZWS_CLI      "build cli"             ON)
option (LZWS_TESTS    "build tests"           ON)
option (LZWS_COVERAGE "enable coverage"       OFF)
option (LZWS_EXAMPLES "build examples"        OFF)
option (LZWS_MAN      "build man"             ON)

if (NOT LZWS_COMPRESSOR_DICTIONARY)
  message (FATAL_ERROR "Please select dictionary implementation.")
endif ()

if (NOT LZWS_SHARED AND NOT LZWS_STATIC)
  message (FATAL_ERROR "Please enable building of shared or static binaries.")
endif ()

set (
  CMAKE_MODULE_PATH
  "${PROJECT_SOURCE_DIR}/cmake/checks"
  "${PROJECT_SOURCE_DIR}/cmake/finds"
  "${PROJECT_SOURCE_DIR}/cmake/functions"
  "${PROJECT_SOURCE_DIR}/cmake/generators"
)

include (GetVerboseFlags)
cmake_get_verbose_flags ()

include (GetPipeFlags)
cmake_get_pipe_flags ()

if (LZWS_COVERAGE)
  include (GetCoverageFlags)
  cmake_get_coverage_flags ()
endif ()

include (CheckC18)
cmake_check_c18 ()

include (CheckIPO)
cmake_check_ipo ()

include (CheckGMP)
cmake_check_gmp ()

if (LZWS_SHARED AND NOT CMAKE_GMP_SHARED_LIBRARY_PATH)
  message (FATAL_ERROR "GMP shared library is not available.")
endif ()
if (LZWS_STATIC AND NOT CMAKE_GMP_STATIC_LIBRARY_PATH)
  message (FATAL_ERROR "GMP static library is not available.")
endif ()

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_VERBOSE_C_FLAGS} ${CMAKE_PIPE_C_FLAGS} ${CMAKE_C18_C_FLAGS}")

if (LZWS_COVERAGE)
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_COVERAGE_C_FLAGS}")
endif ()

if (NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
  set (CMAKE_BUILD_TYPE "DEBUG")
endif ()

set (CMAKE_C_FLAGS_DEBUG         "-O0 -g" CACHE STRING "debug cflags"         FORCE)
set (CMAKE_C_FLAGS_RELEASE       "-O2"    CACHE STRING "release cflags"       FORCE)
set (CMAKE_C_FLAGS_RELEASE_EMBED "-Os"    CACHE STRING "release embed cflags" FORCE)

if (CMAKE_HAVE_IPO)
  set (CMAKE_INTERPROCEDURAL_OPTIMIZATION true CACHE BOOL "status of IPO")
else ()
  set (CMAKE_INTERPROCEDURAL_OPTIMIZATION false CACHE BOOL "status of IPO")
endif ()

if (LZWS_TESTS)
  enable_testing ()
endif ()

add_subdirectory ("src")

if (LZWS_MAN)
  add_subdirectory ("man")
endif ()

include (CPackConfig.cmake)
include (CPack)
