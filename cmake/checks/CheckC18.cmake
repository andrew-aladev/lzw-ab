set (CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

function (cmake_check_c18)
  if (DEFINED CMAKE_HAVE_C18)
    return ()
  endif ()

  set (MESSAGE_PREFIX "Status of C18 support")

  set (NAME "cmake_check_c18")
  set (SOURCE_DIR "${CURRENT_LIST_DIR}/C18")
  set (BINARY_DIR "${PROJECT_BINARY_DIR}/check_c18")

  include (GetVerboseFlags)
  cmake_get_verbose_flags ()

  include (CheckRunnable)
  cmake_check_runnable ()

  # -- -gnu18 --

  try_compile (
    COMPILE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
    CMAKE_FLAGS
      "-DCMAKE_C_FLAGS=${CMAKE_VERBOSE_C_FLAGS} ${CMAKE_WERROR_C_FLAGS} -std=gnu18"
      "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}"
      "-DCMAKE_TRY_RUN=${CMAKE_CAN_RUN_EXE}"
    OUTPUT_VARIABLE COMPILE_OUTPUT
  )
  file (REMOVE_RECURSE ${BINARY_DIR})

  if (CMAKE_VERBOSE_MAKEFILE)
    message (STATUS ${COMPILE_OUTPUT})
  endif ()

  if (COMPILE_RESULT)
    set (CMAKE_HAVE_C18 true)
    set (CMAKE_C18_C_FLAGS "-std=gnu18")
    message (STATUS "${MESSAGE_PREFIX} - gnu18")
  endif ()

  # -- -c18 --

  if (NOT COMPILE_RESULT)
    try_compile (
      COMPILE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
      CMAKE_FLAGS
        "-DCMAKE_C_FLAGS=${CMAKE_VERBOSE_C_FLAGS} ${CMAKE_WERROR_C_FLAGS} -std=c18"
        "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}"
        "-DCMAKE_TRY_RUN=${CMAKE_CAN_RUN_EXE}"
      OUTPUT_VARIABLE COMPILE_OUTPUT
    )
    file (REMOVE_RECURSE ${BINARY_DIR})

    if (CMAKE_VERBOSE_MAKEFILE)
      message (STATUS ${COMPILE_OUTPUT})
    endif ()

    if (COMPILE_RESULT)
      set (CMAKE_HAVE_C18 true)
      set (CMAKE_C18_C_FLAGS "-std=c18")
      message (STATUS "${MESSAGE_PREFIX} - c18")
    endif ()
  endif ()

  # -- vanilla --

  if (NOT COMPILE_RESULT)
    try_compile (
      COMPILE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
      CMAKE_FLAGS
        "-DCMAKE_C_FLAGS=${CMAKE_VERBOSE_C_FLAGS} ${CMAKE_WERROR_C_FLAGS}"
        "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}"
        "-DCMAKE_TRY_RUN=${CMAKE_CAN_RUN_EXE}"
      OUTPUT_VARIABLE COMPILE_OUTPUT
    )
    file (REMOVE_RECURSE ${BINARY_DIR})

    if (CMAKE_VERBOSE_MAKEFILE)
      message (STATUS ${COMPILE_OUTPUT})
    endif ()

    if (COMPILE_RESULT)
      set (CMAKE_HAVE_C18 true)
      set (CMAKE_C18_C_FLAGS "")
      message (STATUS "${MESSAGE_PREFIX} - vanilla")
    endif ()
  endif ()

  # -- no support --

  if (NOT COMPILE_RESULT)
    set (CMAKE_HAVE_C18 false)
    set (CMAKE_C18_C_FLAGS "")
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  set (CMAKE_HAVE_C18 ${CMAKE_HAVE_C18} CACHE STRING "Status of C18")
  set (CMAKE_C18_C_FLAGS ${CMAKE_C18_C_FLAGS} CACHE STRING "C18 C flags")

  mark_as_advanced (CMAKE_HAVE_C18 CMAKE_C18_C_FLAGS)
endfunction ()
