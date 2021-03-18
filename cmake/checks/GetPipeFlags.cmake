set (CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

function (cmake_get_pipe_flags)
  if (DEFINED CMAKE_GET_PIPE_FLAGS_PROCESSED)
    return ()
  endif ()

  set (MESSAGE_PREFIX "Status of pipe support")

  if (NOT MSVC)
    set (NAME "cmake_get_pipe_flags")
    set (SOURCE_DIR "${CURRENT_LIST_DIR}/basic")
    set (BINARY_DIR "${PROJECT_BINARY_DIR}/get_pipe_flags")

    include (GetVerboseFlags)
    cmake_get_verbose_flags ()

    include (CheckRunnable)
    cmake_check_runnable ()

    set (FLAG "-pipe")

    try_compile (
      COMPILE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
      CMAKE_FLAGS
        "-DCMAKE_C_FLAGS=${CMAKE_WERROR_C_FLAGS} ${FLAG}"
        "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}"
        "-DCMAKE_TRY_RUN=${CMAKE_CAN_RUN_EXE}"
      OUTPUT_VARIABLE COMPILE_OUTPUT
    )
    file (REMOVE_RECURSE ${BINARY_DIR})

    if (CMAKE_VERBOSE_MAKEFILE)
      message (STATUS ${COMPILE_OUTPUT})
    endif ()

    if (COMPILE_RESULT)
      set (CMAKE_PIPE_C_FLAGS ${FLAG})
      message (STATUS "${MESSAGE_PREFIX} - ${FLAG}")
    else ()
      set (CMAKE_PIPE_C_FLAGS "")
      message (STATUS "${MESSAGE_PREFIX} - no")
    endif ()

  else ()
    set (CMAKE_PIPE_C_FLAGS "")
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  set (CMAKE_PIPE_C_FLAGS ${CMAKE_PIPE_C_FLAGS} CACHE STRING "Pipe C flags")
  set (CMAKE_GET_PIPE_FLAGS_PROCESSED true CACHE STRING "Pipe flags processed")

  mark_as_advanced (CMAKE_PIPE_C_FLAGS CMAKE_GET_PIPE_FLAGS_PROCESSED)
endfunction ()
