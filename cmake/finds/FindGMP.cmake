if (DEFINED CMAKE_GMP_FOUND)
  return ()
endif ()

set (MESSAGE_PREFIX "Status of GMP")

find_path (CMAKE_GMP_INCLUDE_PATH NAMES "gmp.h")
if (CMAKE_GMP_INCLUDE_PATH)
  message (STATUS "${MESSAGE_PREFIX} - header file found: \"${CMAKE_GMP_INCLUDE_PATH}\"")
else ()
  message (STATUS "${MESSAGE_PREFIX} - header file not found")
endif ()

find_library (
  CMAKE_GMP_SHARED_LIBRARY_PATH NAMES
  "libgmp.so" "libgmp.dylib" "libgmp.dll"
  "gmp.so" "gmp.dylib" "gmp.dll"
)
if (CMAKE_GMP_SHARED_LIBRARY_PATH)
  message (STATUS "${MESSAGE_PREFIX} - shared library found: \"${CMAKE_GMP_SHARED_LIBRARY_PATH}\"")
else ()
  message (STATUS "${MESSAGE_PREFIX} - shared library not found")
endif ()

find_library (
  CMAKE_GMP_STATIC_LIBRARY_PATH NAMES
  "libgmp.a" "libgmp.lib"
  "gmp.a" "gmp.lib"
)
if (CMAKE_GMP_STATIC_LIBRARY_PATH)
  message (STATUS "${MESSAGE_PREFIX} - static library found: \"${CMAKE_GMP_STATIC_LIBRARY_PATH}\"")
else ()
  message (STATUS "${MESSAGE_PREFIX} - static library not found")
endif ()

if (
  CMAKE_GMP_INCLUDE_PATH AND
  (CMAKE_GMP_SHARED_LIBRARY_PATH OR CMAKE_GMP_STATIC_LIBRARY_PATH)
)
  set (CMAKE_GMP_FOUND true)
else ()
  set (CMAKE_GMP_FOUND false)
endif ()

set (
  CMAKE_GMP_FOUND ${CMAKE_GMP_FOUND}
  CACHE BOOL "GMP found"
)

mark_as_advanced (
  CMAKE_GMP_FOUND
  CMAKE_GMP_INCLUDE_PATH
  CMAKE_GMP_SHARED_LIBRARY_PATH
  CMAKE_GMP_STATIC_LIBRARY_PATH
)

if (NOT CMAKE_GMP_FOUND AND GMP_FIND_REQUIRED)
  message (FATAL_ERROR "${MESSAGE_PREFIX} - is required")
endif ()
