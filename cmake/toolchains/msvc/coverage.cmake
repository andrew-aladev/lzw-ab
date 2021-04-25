include ("${CMAKE_CURRENT_LIST_DIR}/default.cmake")

if (CMAKE_GENERATOR MATCHES "Visual Studio")
  set (
    CMAKE_DEFAULT_C_FLAGS ${CMAKE_C_FLAGS}
    CACHE STRING "cmake default C flags"
  )
  set (
    CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Ob0"
    CACHE STRING "cmake C flags"
  )
else ()
  message (FATAL_ERROR "msvc coverage toolchain is not supported")
endif ()
