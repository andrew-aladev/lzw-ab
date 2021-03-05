set (CMAKE_C_COMPILER "gcc")

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -Wno-maybe-uninitialized" CACHE STRING "C flags")

set (CMAKE_HAVE_IPO false CACHE STRING "IPO disabled")
