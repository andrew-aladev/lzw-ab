set (CMAKE_C_COMPILER "gcc")

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=leak" CACHE STRING "cmake C flags")
