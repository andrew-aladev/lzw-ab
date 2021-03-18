set (CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

function (cmake_get_build_flags)
  if (DEFINED CMAKE_GET_BUILD_FLAGS_PROCESSED)
    return ()
  endif ()

  set (NAME "cmake_get_build_flags")
  set (SOURCE_DIR "${CURRENT_LIST_DIR}/basic")
  set (BINARY_DIR "${PROJECT_BINARY_DIR}/get_build_flags")

  include (GetVerboseFlags)
  cmake_get_verbose_flags ()

  include (CheckRunnable)
  cmake_check_runnable ()

  # -- Debug --

  if (MSVC)
    set (FLAGS "/Od" "/Zi")
  else ()
    set (FLAGS "-O0" "-g")
  endif ()

  set (MESSAGE_PREFIX "Status of Debug support")

  foreach (FLAG ${FLAGS})
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
      set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${FLAG}")
      message (STATUS "${MESSAGE_PREFIX} - ${FLAG}")
    endif ()
  endforeach ()

  if (NOT COMPILE_RESULT)
    set (CMAKE_C_FLAGS_DEBUG "")
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  set (CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG} CACHE STRING "Debug C flags" FORCE)

  # -- Release --

  if (MSVC)
    set (FLAGS "/O2")
  else ()
    set (FLAGS "-O2")
  endif ()

  set (MESSAGE_PREFIX "Status of Release support")

  foreach (FLAG ${FLAGS})
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
      set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${FLAG}")
      message (STATUS "${MESSAGE_PREFIX} - ${FLAG}")
    endif ()
  endforeach ()

  if (NOT COMPILE_RESULT)
    set (CMAKE_C_FLAGS_RELEASE "")
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  set (CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE} CACHE STRING "Release C flags" FORCE)

  # -- RelWithDebInfo --

  if (MSVC)
    set (FLAGS "/O2" "/Zi")
  else ()
    set (FLAGS "-O2" "-g")
  endif ()

  set (MESSAGE_PREFIX "Status of RelWithDebInfo support")

  foreach (FLAG ${FLAGS})
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
      set (CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} ${FLAG}")
      message (STATUS "${MESSAGE_PREFIX} - ${FLAG}")
    endif ()
  endforeach ()

  if (NOT COMPILE_RESULT)
    set (CMAKE_C_FLAGS_RELWITHDEBINFO "")
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  set (CMAKE_C_FLAGS_RELWITHDEBINFO ${CMAKE_C_FLAGS_RELWITHDEBINFO} CACHE STRING "RelWithDebInfo C flags" FORCE)

  # -- MinSizeRel --

  if (MSVC)
    set (FLAGS "/Os")
  else ()
    set (FLAGS "-Os")
  endif ()

  set (MESSAGE_PREFIX "Status of MinSizeRel support")

  foreach (FLAG ${FLAGS})
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
      set (CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} ${FLAG}")
      message (STATUS "${MESSAGE_PREFIX} - ${FLAG}")
    endif ()
  endforeach ()

  if (NOT COMPILE_RESULT)
    set (CMAKE_C_FLAGS_MINSIZEREL "")
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  set (CMAKE_C_FLAGS_MINSIZEREL ${CMAKE_C_FLAGS_MINSIZEREL} CACHE STRING "MinSizeRel C flags" FORCE)

  mark_as_advanced (
    CMAKE_C_FLAGS_DEBUG
    CMAKE_C_FLAGS_RELEASE
    CMAKE_C_FLAGS_RELWITHDEBINFO
    CMAKE_C_FLAGS_MINSIZEREL
  )
endfunction ()
