set (CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

function (cmake_get_verbose_flags)
  if (DEFINED CMAKE_GET_VERBOSE_FLAGS_PROCESSED)
    return ()
  endif ()

  set (NAME "cmake_get_verbose_flags")
  set (SOURCE_DIR "${CURRENT_LIST_DIR}/basic")
  set (BINARY_DIR "${PROJECT_BINARY_DIR}/get_verbose_flags")

  # -- Werror --

  set (FLAGS "-Werror" "/WX")

  set (MESSAGE_PREFIX "Status of Werror support")

  foreach (FLAG ${FLAGS})
    try_compile (
      COMPILE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
      CMAKE_FLAGS
        "-DCMAKE_C_FLAGS=${FLAG}"
        "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}"
      OUTPUT_VARIABLE COMPILE_OUTPUT
    )
    file (REMOVE_RECURSE ${BINARY_DIR})

    if (CMAKE_VERBOSE_MAKEFILE)
      message (STATUS ${COMPILE_OUTPUT})
    endif ()

    if (COMPILE_RESULT)
      set (CMAKE_WERROR_C_FLAGS ${FLAG})
      message (STATUS "${MESSAGE_PREFIX} - ${FLAG}")

      break ()
    endif ()
  endforeach ()

  if (NOT COMPILE_RESULT)
    set (CMAKE_WERROR_C_FLAGS "")
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  set (CMAKE_WERROR_C_FLAGS ${CMAKE_WERROR_C_FLAGS} CACHE STRING "Werror C flags")

  # -- pedantic --

  set (MESSAGE_PREFIX "Status of -pedantic support")

  set (CMAKE_VERBOSE_C_FLAGS "")

  try_compile (
    COMPILE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
    CMAKE_FLAGS
      "-DCMAKE_C_FLAGS=${CMAKE_WERROR_C_FLAGS} -pedantic"
      "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}"
    OUTPUT_VARIABLE COMPILE_OUTPUT
  )
  file (REMOVE_RECURSE ${BINARY_DIR})

  if (CMAKE_VERBOSE_MAKEFILE)
    message (STATUS ${COMPILE_OUTPUT})
  endif ()

  if (COMPILE_RESULT)
    set (CMAKE_VERBOSE_C_FLAGS "${CMAKE_VERBOSE_C_FLAGS} -pedantic")
    message (STATUS "${MESSAGE_PREFIX} - yes")
  else ()
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  # -- Wall --

  set (MESSAGE_PREFIX "Status of -Wall support")

  try_compile (
    COMPILE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
    CMAKE_FLAGS
      "-DCMAKE_C_FLAGS=${CMAKE_WERROR_C_FLAGS} -Wall"
      "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}"
    OUTPUT_VARIABLE COMPILE_OUTPUT
  )
  file (REMOVE_RECURSE ${BINARY_DIR})

  if (CMAKE_VERBOSE_MAKEFILE)
    message (STATUS ${COMPILE_OUTPUT})
  endif ()

  if (COMPILE_RESULT)
    set (CMAKE_VERBOSE_C_FLAGS "${CMAKE_VERBOSE_C_FLAGS} -Wall")
    message (STATUS "${MESSAGE_PREFIX} - yes")
  else ()
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  # -- Wextra --

  set (MESSAGE_PREFIX "Status of -Wextra support")

  try_compile (
    COMPILE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
    CMAKE_FLAGS
      "-DCMAKE_C_FLAGS=${CMAKE_WERROR_C_FLAGS} -Wextra"
      "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}"
    OUTPUT_VARIABLE COMPILE_OUTPUT
  )
  file (REMOVE_RECURSE ${BINARY_DIR})

  if (CMAKE_VERBOSE_MAKEFILE)
    message (STATUS ${COMPILE_OUTPUT})
  endif ()

  if (COMPILE_RESULT)
    set (CMAKE_VERBOSE_C_FLAGS "${CMAKE_VERBOSE_C_FLAGS} -Wextra")
    message (STATUS "${MESSAGE_PREFIX} - yes")
  else ()
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  set (CMAKE_VERBOSE_C_FLAGS ${CMAKE_VERBOSE_C_FLAGS} CACHE STRING "Verbose C flags")
  set (CMAKE_GET_VERBOSE_FLAGS_PROCESSED true CACHE STRING "Verbose flags processed")

  mark_as_advanced (
    CMAKE_WERROR_C_FLAGS
    CMAKE_VERBOSE_C_FLAGS
    CMAKE_GET_VERBOSE_FLAGS_PROCESSED
  )
endfunction ()
