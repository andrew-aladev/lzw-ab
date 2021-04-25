include ("${CMAKE_CURRENT_LIST_DIR}/default.cmake")

if (CMAKE_GENERATOR MATCHES "Visual Studio")
  message (FATAL_ERROR "gcc coverage toolchain is not supported")
else ()
  set (
    CMAKE_DEFAULT_C_FLAGS ${CMAKE_C_FLAGS}
    CACHE STRING "cmake default C flags"
  )
  set (
    CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-inline"
    CACHE STRING "cmake C flags"
  )
endif ()
