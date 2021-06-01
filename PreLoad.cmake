# Cmake uses Visual Studio generator for MinGW by default (even without Visual Studio).
# It will be better to select another default generator for MinGW.
if (NOT "$ENV{MSYSTEM}" STREQUAL "" AND "$ENV{VisualStudioVersion}" STREQUAL "")
  find_program (CMAKE_NINJA_BINARY NAMES "ninja")
  if (CMAKE_NINJA_BINARY)
    set (
      CMAKE_GENERATOR "Ninja"
      CACHE INTERNAL "Cmake generator"
    )
    return ()
  endif ()

  find_program (CMAKE_MAKE_BINARY NAMES "gmake" "make")
  if (CMAKE_MAKE_BINARY)
    set (
      CMAKE_GENERATOR "Unix Makefiles"
      CACHE INTERNAL "Cmake generator"
    )
    return ()
  endif ()
endif ()
