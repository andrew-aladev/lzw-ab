# Cmake uses Visual Studio generator for MinGW by default.
# It will be better to select another default generator.
if (NOT "$ENV{MSYSTEM}" STREQUAL "")
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
