if (CMAKE_GENERATOR MATCHES "Visual Studio")
  message (FATAL_ERROR "gcc toolchain is not supported")
else ()
  set (
    CMAKE_DEFAULT_C_COMPILER ${CMAKE_C_COMPILER}
    CACHE STRING "cmake default C compiler"
  )
  set (
    CMAKE_C_COMPILER "gcc"
    CACHE STRING "cmake C compiler"
  )
endif ()
