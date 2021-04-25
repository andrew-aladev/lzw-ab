include ("${CMAKE_CURRENT_LIST_DIR}/default.cmake")

set (
  CMAKE_DEFAULT_C_FLAGS ${CMAKE_C_FLAGS}
  CACHE STRING "cmake default C flags"
)

if (CMAKE_GENERATOR MATCHES "Visual Studio")
  set (
    CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /fsanitize=address"
    CACHE STRING "cmake C flags"
  )
else ()
  set (
    CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address"
    CACHE STRING "cmake C flags"
  )
endif ()
