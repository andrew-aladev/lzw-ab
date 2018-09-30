cmake_minimum_required(VERSION 3.9)
project ("lzw-ab" "C")

if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not allowed")
endif ()

set (LZW_AB_VERSION "0.1")

if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set (CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "output directory for libraries")
endif ()
if (NOT DEFINED CMAKE_INSTALL_INCLUDEDIR)
  set (CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "output directory for include files")
endif ()
if (NOT DEFINED CMAKE_INSTALL_BINDIR)
  set (CMAKE_INSTALL_BINDIR "bin" CACHE PATH "output directory for binaries")
endif ()

if (NOT DEFINED LZW_AB_TARGET)
  set (LZW_AB_TARGET ${PROJECT_NAME})
endif ()

set (LZW_AB_CLI    true CACHE BOOL "build cli")
set (LZW_AB_SHARED true CACHE BOOL "build shared libs")
set (LZW_AB_STATIC true CACHE BOOL "build static libs")

if (NOT LZW_AB_STATIC AND NOT LZW_AB_SHARED)
  message (FATAL_ERROR "LZW AB can not be compiled. Please enable building of shared or static libraries.")
endif ()

set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/checks")

include (GetVerboseFlags)
include (GetPipeFlags)
include (CheckC11)
include (CheckIPO)
include (CheckRunnable)

cmake_get_verbose_flags ()
cmake_get_pipe_flags ()
cmake_check_c11 ()
cmake_check_ipo ()
cmake_check_runnable ()

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_PIPE_C_FLAGS} ${CMAKE_VERBOSE_C_FLAGS} ${CMAKE_C11_C_FLAGS}")

if (NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
  set (CMAKE_BUILD_TYPE DEBUG)
endif ()

set (CMAKE_C_FLAGS_DEBUG "-O0 -g")
set (CMAKE_C_FLAGS_RELEASE "-O2")
set (CMAKE_C_FLAGS_RELEASE_EMBED "-Os")

if (
  NOT DEFINED CMAKE_INTERPROCEDURAL_OPTIMIZATION AND
  CMAKE_HAVE_IPO AND (
    CMAKE_BUILD_TYPE MATCHES RELEASE OR
    CMAKE_BUILD_TYPE MATCHES RELEASE_EMBED
  )
)
  set (CMAKE_INTERPROCEDURAL_OPTIMIZATION true CACHE BOOL "status of IPO switch")
endif ()

include_directories ("src")
add_subdirectory ("src")

include (CPackConfig.cmake)
include (CPack)