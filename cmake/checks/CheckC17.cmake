set (CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

function (cmake_check_c17)
  if (DEFINED CMAKE_HAVE_C17)
    return ()
  endif ()

  set (MESSAGE_PREFIX "Status of C17 support")

  set (NAME "cmake_check_c17")
  set (SOURCE_DIR "${CURRENT_LIST_DIR}/C17")
  set (BINARY_DIR "${PROJECT_BINARY_DIR}/check_c17")

  include (GetVerboseFlags)
  cmake_get_verbose_flags ()

  include (CheckRunnable)
  cmake_check_runnable ()

  # -- trying multiple flags --

  set (FLAGS "-std=gnu18" "-std=c18" "-std=gnu17" "-std=c17" "/std:c17" "")

  foreach (FLAG ${FLAGS})
    try_compile (
      COMPILE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
      CMAKE_FLAGS
        "-DCMAKE_C_FLAGS=${CMAKE_VERBOSE_C_FLAGS} ${FLAG}"
        "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}"
        "-DCMAKE_TRY_RUN=${CMAKE_CAN_RUN_EXE}"
      OUTPUT_VARIABLE COMPILE_OUTPUT
    )
    file (REMOVE_RECURSE ${BINARY_DIR})

    if (CMAKE_VERBOSE_MAKEFILE)
      message (STATUS ${COMPILE_OUTPUT})
    endif ()

    if (COMPILE_RESULT)
      set (CMAKE_HAVE_C17 true)
      set (CMAKE_C17_C_FLAGS ${FLAG})

      if (FLAG STREQUAL "")
        set (FLAG "vanilla")
      endif ()

      message (STATUS "${MESSAGE_PREFIX} - ${FLAG}")

      break()
    endif ()
  endforeach ()

  # -- no support --

  if (NOT COMPILE_RESULT)
    set (CMAKE_HAVE_C17 false)
    set (CMAKE_C17_C_FLAGS "")
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  set (CMAKE_HAVE_C17 ${CMAKE_HAVE_C17} CACHE STRING "Status of C17")
  set (CMAKE_C17_C_FLAGS ${CMAKE_C17_C_FLAGS} CACHE STRING "C17 C flags")

  mark_as_advanced (CMAKE_HAVE_C17 CMAKE_C17_C_FLAGS)
endfunction ()
