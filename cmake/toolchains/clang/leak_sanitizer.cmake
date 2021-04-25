include ("${CMAKE_CURRENT_LIST_DIR}/default.cmake")

set (
  CMAKE_DEFAULT_C_FLAGS ${CMAKE_C_FLAGS}
  CACHE STRING "cmake default C flags"
)
set (
  CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=leak"
  CACHE STRING "cmake C flags"
)
