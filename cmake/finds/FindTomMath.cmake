if (DEFINED CMAKE_TOMMATH_FOUND)
  return ()
endif ()

set (MESSAGE_PREFIX "Status of TomMath")

find_path (CMAKE_TOMMATH_INCLUDE_PATH NAMES "tommath.h")
if (CMAKE_TOMMATH_INCLUDE_PATH)
  message (STATUS "${MESSAGE_PREFIX} - found: \"${CMAKE_TOMMATH_INCLUDE_PATH}\"")
else ()
  message (STATUS "${MESSAGE_PREFIX} - header file not found")
endif ()

find_library (
  CMAKE_TOMMATH_SHARED_LIBRARY_PATH NAMES
  "libtommath.so" "libtommath.dylib" "libtommath.dll"
  "tommath.so" "tommath.dylib" "tommath.dll"
)
if (CMAKE_TOMMATH_SHARED_LIBRARY_PATH)
  message (STATUS "${MESSAGE_PREFIX} - found: \"${CMAKE_TOMMATH_SHARED_LIBRARY_PATH}\"")
else ()
  message (STATUS "${MESSAGE_PREFIX} - shared library not found")
endif ()

find_library (
  CMAKE_TOMMATH_STATIC_LIBRARY_PATH NAMES
  "libtommath.a" "libtommath.lib"
  "tommath.a" "tommath.lib"
)
if (CMAKE_TOMMATH_STATIC_LIBRARY_PATH)
  message (STATUS "${MESSAGE_PREFIX} - found: \"${CMAKE_TOMMATH_STATIC_LIBRARY_PATH}\"")
else ()
  message (STATUS "${MESSAGE_PREFIX} - static library not found")
endif ()

if (
  CMAKE_TOMMATH_INCLUDE_PATH AND
  (CMAKE_TOMMATH_SHARED_LIBRARY_PATH OR CMAKE_TOMMATH_STATIC_LIBRARY_PATH)
)
  set (CMAKE_TOMMATH_FOUND true)
else ()
  set (CMAKE_TOMMATH_FOUND false)
endif ()

set (
  CMAKE_TOMMATH_FOUND ${CMAKE_TOMMATH_FOUND}
  CACHE BOOL "TomMath found"
)

mark_as_advanced (
  CMAKE_TOMMATH_FOUND
  CMAKE_TOMMATH_INCLUDE_PATH
  CMAKE_TOMMATH_SHARED_LIBRARY_PATH
  CMAKE_TOMMATH_STATIC_LIBRARY_PATH
)

if (NOT CMAKE_TOMMATH_FOUND AND TOMMATH_FIND_REQUIRED)
  message (FATAL_ERROR "${MESSAGE_PREFIX} - is required")
endif ()
