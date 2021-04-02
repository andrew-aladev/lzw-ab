if (CYGWIN OR MSYS OR MINGW)
  set (
    CMAKE_GENERATOR "Unix Makefiles"
    CACHE INTERNAL "Cmake generator" FORCE
  )
elseif (WIN32)
  set (
    CMAKE_GENERATOR "NMake Makefiles"
    CACHE INTERNAL "Cmake generator" FORCE
  )
endif ()
