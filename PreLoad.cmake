if (NOT MSVC)
  set (
    CMAKE_GENERATOR "Unix Makefiles"
    CACHE INTERNAL "Cmake generator"
  )
endif ()
