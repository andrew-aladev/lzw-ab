# We need to set default values in order to override values from any other toolchain.

if (DEFINED CMAKE_DEFAULT_C_COMPILER)
  set (
    CMAKE_C_COMPILER ${CMAKE_DEFAULT_C_COMPILER}
    CACHE STRING "cmake C compiler"
  )
endif ()

if (DEFINED CMAKE_DEFAULT_C_COMPILER_AR)
  set (
    CMAKE_C_COMPILER_AR ${CMAKE_DEFAULT_C_COMPILER_AR}
    CACHE STRING "cmake C compiler ar"
  )
endif ()

if (DEFINED CMAKE_DEFAULT_C_COMPILER_RANLIB)
  set (
    CMAKE_C_COMPILER_RANLIB ${CMAKE_DEFAULT_C_COMPILER_RANLIB}
    CACHE STRING "cmake C compiler ranlib"
  )
endif ()

if (DEFINED CMAKE_DEFAULT_C_FLAGS)
  set (
    CMAKE_C_FLAGS ${CMAKE_DEFAULT_C_FLAGS}
    CACHE STRING "cmake C flags"
  )
endif ()
