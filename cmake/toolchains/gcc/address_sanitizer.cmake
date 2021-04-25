include ("${CMAKE_CURRENT_LIST_DIR}/default.cmake")

if (NOT CMAKE_GENERATOR MATCHES "Visual Studio")
  set (
    CMAKE_DEFAULT_C_FLAGS ${CMAKE_C_FLAGS}
    CACHE STRING "cmake default C flags"
  )
  set (
    CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -Wno-maybe-uninitialized"
    CACHE STRING "cmake C flags"
  )
endif ()
