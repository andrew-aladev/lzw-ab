if ("$ENV{VisualStudioVersion}" STREQUAL "")
  set (
    CMAKE_GENERATOR "Unix Makefiles"
    CACHE INTERNAL "Cmake generator"
  )
endif ()
