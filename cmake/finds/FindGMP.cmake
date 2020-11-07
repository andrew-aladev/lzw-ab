if (DEFINED CMAKE_GMP_FOUND)
  return ()
endif ()

set (MESSAGE_PREFIX "Status of GMP")

find_path (CMAKE_GMP_INCLUDE_PATH NAMES "gmp.h")
if (CMAKE_GMP_INCLUDE_PATH)
  message (STATUS "${MESSAGE_PREFIX} - found: \"${CMAKE_GMP_INCLUDE_PATH}\"")
else ()
  message (STATUS "${MESSAGE_PREFIX} - header file not found")
endif ()

find_library (CMAKE_GMP_SHARED_LIBRARY_PATH NAMES "libgmp.so" "libgmp.dylib" "libgmp.dll")
if (CMAKE_GMP_SHARED_LIBRARY_PATH)
  message (STATUS "${MESSAGE_PREFIX} - found: \"${CMAKE_GMP_SHARED_LIBRARY_PATH}\"")
else ()
  message (STATUS "${MESSAGE_PREFIX} - shared library not found")
endif ()

find_library (CMAKE_GMP_STATIC_LIBRARY_PATH NAMES "libgmp.a")
if (CMAKE_GMP_STATIC_LIBRARY_PATH)
  message (STATUS "${MESSAGE_PREFIX} - found: \"${CMAKE_GMP_STATIC_LIBRARY_PATH}\"")
else ()
  message (STATUS "${MESSAGE_PREFIX} - static library not found")
endif ()

set (
  CMAKE_GMP_FOUND
  (CMAKE_GMP_INCLUDE_PATH AND (CMAKE_GMP_SHARED_LIBRARY_PATH OR CMAKE_GMP_STATIC_LIBRARY_PATH))
  CACHE STRING "GMP found"
)

if (NOT CMAKE_GMP_FOUND AND GMP_FIND_REQUIRED)
  message (FATAL_ERROR "${MESSAGE_PREFIX} - GMP is required")
endif ()

mark_as_advanced (CMAKE_GMP_FOUND CMAKE_GMP_INCLUDE_PATH CMAKE_GMP_SHARED_LIBRARY_PATH CMAKE_GMP_STATIC_LIBRARY_PATH)
