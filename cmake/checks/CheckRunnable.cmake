set (CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

function (cmake_check_runnable)
  if (DEFINED CMAKE_CAN_RUN_EXE)
    return ()
  endif ()

  set (NAME "cmake_check_runnable")
  set (SOURCE_DIR "${CURRENT_LIST_DIR}/basic")
  set (BINARY_DIR "${PROJECT_BINARY_DIR}/check_runnable")

  include (GetVerboseFlags)
  cmake_get_verbose_flags ()

  try_compile (
    COMPILE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
    CMAKE_FLAGS
      "-DCMAKE_C_FLAGS=${CMAKE_WERROR_C_FLAGS}"
      "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}"
      "-DCMAKE_TRY_RUN=1"
    OUTPUT_VARIABLE COMPILE_OUTPUT
  )
  file (REMOVE_RECURSE ${BINARY_DIR})

  if (CMAKE_VERBOSE_MAKEFILE)
    message (STATUS ${COMPILE_OUTPUT})
  endif ()

  set (MESSAGE_PREFIX "Status of run exe support")

  if (COMPILE_RESULT)
    set (CMAKE_CAN_RUN_EXE true)
    message (STATUS "${MESSAGE_PREFIX} - working")
  else ()
    set (CMAKE_CAN_RUN_EXE false)
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  set (CMAKE_CAN_RUN_EXE ${CMAKE_CAN_RUN_EXE} CACHE STRING "Status of run exe")

  mark_as_advanced (CMAKE_CAN_RUN_EXE)
endfunction ()
