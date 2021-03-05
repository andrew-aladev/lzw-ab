set (CMAKE_C_COMPILER "clang")
set (CMAKE_C_COMPILER_AR "llvm-ar")
set (CMAKE_C_COMPILER_RANLIB "llvm-ranlib")

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address" CACHE STRING "C flags")

set (CMAKE_HAVE_IPO false CACHE STRING "IPO disabled")
