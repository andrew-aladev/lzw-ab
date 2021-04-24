if (CMAKE_GENERATOR MATCHES "Visual Studio")
  set (
    CMAKE_DEFAULT_GENERATOR_TOOLSET ${CMAKE_GENERATOR_TOOLSET}
    CACHE STRING "cmake default generator toolset"
  )
  set (
    CMAKE_GENERATOR_TOOLSET ""
    CACHE STRING "cmake generator toolset"
  )
else ()
  message (FATAL_ERROR "msvc toolchain is not supported")
endif ()
