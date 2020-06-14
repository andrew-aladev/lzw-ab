if (NOT CMAKE_GENERATOR_USE_CURRENT_TOOLCHAIN)
  include ("${CMAKE_CURRENT_LIST_DIR}/../../toolchains/default.cmake")
endif ()

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
