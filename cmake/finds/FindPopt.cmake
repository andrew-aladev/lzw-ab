if (DEFINED CMAKE_POPT_FOUND)
  return ()
endif ()

set (MESSAGE_PREFIX "Status of Popt")

find_path (CMAKE_POPT_INCLUDE_PATH NAMES "popt.h")
if (CMAKE_POPT_INCLUDE_PATH)
  message (STATUS "${MESSAGE_PREFIX} - found: \"${CMAKE_POPT_INCLUDE_PATH}\"")
else ()
  message (STATUS "${MESSAGE_PREFIX} - header file not found")
endif ()

find_library (
  CMAKE_POPT_SHARED_LIBRARY_PATH NAMES
  "libpopt.so" "libpopt.dylib" "libpopt.dll"
  "popt.so" "popt.dylib" "popt.dll"
)
if (CMAKE_POPT_SHARED_LIBRARY_PATH)
  message (STATUS "${MESSAGE_PREFIX} - found: \"${CMAKE_POPT_SHARED_LIBRARY_PATH}\"")
else ()
  message (STATUS "${MESSAGE_PREFIX} - shared library not found")
endif ()

find_library (
  CMAKE_POPT_STATIC_LIBRARY_PATH NAMES
  "libpopt.a" "libpopt.lib"
  "popt.a" "popt.lib"
)
if (CMAKE_POPT_STATIC_LIBRARY_PATH)
  message (STATUS "${MESSAGE_PREFIX} - found: \"${CMAKE_POPT_STATIC_LIBRARY_PATH}\"")
else ()
  message (STATUS "${MESSAGE_PREFIX} - static library not found")
endif ()

if (
  CMAKE_POPT_INCLUDE_PATH AND
  (CMAKE_POPT_SHARED_LIBRARY_PATH OR CMAKE_POPT_STATIC_LIBRARY_PATH)
)
  set (CMAKE_POPT_FOUND true)
else ()
  set (CMAKE_POPT_FOUND false)
endif ()

set (
  CMAKE_POPT_FOUND ${CMAKE_POPT_FOUND}
  CACHE BOOL "Popt found"
)

mark_as_advanced (
  CMAKE_POPT_FOUND
  CMAKE_POPT_INCLUDE_PATH
  CMAKE_POPT_SHARED_LIBRARY_PATH
  CMAKE_POPT_STATIC_LIBRARY_PATH
)

if (NOT CMAKE_POPT_FOUND AND Popt_FIND_REQUIRED)
  message (FATAL_ERROR "${MESSAGE_PREFIX} - is required")
endif ()
