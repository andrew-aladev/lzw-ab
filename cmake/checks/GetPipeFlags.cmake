function (cmake_get_pipe_flags)
  if (DEFINED CMAKE_GET_PIPE_FLAGS_PROCESSED)
    return ()
  endif ()

  include (GetVerboseFlags)
  cmake_get_verbose_flags ()

  set (BINARY_DIR "${PROJECT_BINARY_DIR}/CMakeTmp/check_basic")
  set (SOURCE_DIR "${PROJECT_SOURCE_DIR}/cmake/checks/basic")
  set (NAME "cmake_check_basic")

  try_compile (
    CHECK_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
    CMAKE_FLAGS
      "-DCMAKE_C_FLAGS=${CMAKE_C_FLAGS} ${CMAKE_VERBOSE_C_FLAGS} ${CMAKE_WERROR_C_FLAGS} -pipe"
      "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}"
    OUTPUT_VARIABLE CHECK_OUTPUT
  )
  if (CMAKE_VERBOSE_MAKEFILE)
    message (STATUS ${CHECK_OUTPUT})
  endif ()
  FILE (REMOVE_RECURSE ${BINARY_DIR})

  if (CHECK_RESULT)
    set (CMAKE_PIPE_C_FLAGS "-pipe" CACHE STRING "pipe C flags")
    message (STATUS "Status of -pipe support - yes")
  else ()
    set (CMAKE_PIPE_C_FLAGS "" CACHE STRING "pipe C flags")
    message (STATUS "Status of -pipe support - no")
  endif ()

  set (CMAKE_GET_PIPE_FLAGS_PROCESSED true CACHE STRING "get pipe flags processed")

  mark_as_advanced(CMAKE_GET_PIPE_FLAGS_PROCESSED CMAKE_PIPE_C_FLAGS)
endfunction ()
