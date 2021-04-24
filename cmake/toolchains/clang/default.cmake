if (CMAKE_GENERATOR MATCHES "Visual Studio")
  set (
    CMAKE_DEFAULT_GENERATOR_TOOLSET ${CMAKE_GENERATOR_TOOLSET}
    CACHE STRING "cmake default generator toolset"
  )
  set (
    CMAKE_GENERATOR_TOOLSET "clangcl"
    CACHE STRING "cmake generator toolset"
  )
else ()
  set (
    CMAKE_DEFAULT_C_COMPILER ${CMAKE_C_COMPILER}
    CACHE STRING "cmake default C compiler"
  )
  set (
    CMAKE_C_COMPILER "clang"
    CACHE STRING "cmake C compiler"
  )

  set (
    CMAKE_DEFAULT_C_COMPILER_AR ${CMAKE_C_COMPILER_AR}
    CACHE STRING "cmake default C compiler ar"
  )
  set (
    CMAKE_C_COMPILER_AR "llvm-ar"
    CACHE STRING "cmake C compiler ar"
  )

  set (
    CMAKE_DEFAULT_C_COMPILER_RANLIB ${CMAKE_C_COMPILER_RANLIB}
    CACHE STRING "cmake default C compiler ranlib"
  )
  set (
    CMAKE_C_COMPILER_RANLIB "llvm-ranlib"
    CACHE STRING "cmake C compiler ranlib"
  )
endif ()
