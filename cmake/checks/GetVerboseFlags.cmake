set (CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

function (cmake_test_verbose_flag FLAG)
  set (NAME "cmake_get_verbose_flags")
  set (SOURCE_DIR "${CURRENT_LIST_DIR}/basic")
  set (BINARY_DIR "${PROJECT_BINARY_DIR}/get_verbose_flags")

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

  set (COMPILE_RESULT ${COMPILE_RESULT} PARENT_SCOPE)
endfunction ()

function (cmake_get_verbose_flags)
  if (DEFINED CMAKE_GET_VERBOSE_FLAGS_PROCESSED)
    return ()
  endif ()

  # -- Werror --

  if (MSVC)
    set (FLAG "/WX")
  else ()
    set (FLAG "-Werror")
  endif ()

  cmake_test_verbose_flag (${FLAG})

  set (MESSAGE_PREFIX "Status of Werror support")

  if (COMPILE_RESULT)
    set (CMAKE_WERROR_C_FLAGS ${FLAG})
    message (STATUS "${MESSAGE_PREFIX} - ${FLAG}")
  else ()
    set (CMAKE_WERROR_C_FLAGS "")
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  set (CMAKE_WERROR_C_FLAGS ${CMAKE_WERROR_C_FLAGS} CACHE STRING "Werror C flags")

  # -- pedantic --

  if (MSVC)
    set (FLAG "/permissive-")
  else ()
    set (FLAG "-pedantic")
  endif ()

  set (CMAKE_VERBOSE_C_FLAGS "")

  cmake_test_verbose_flag (${FLAG})

  set (MESSAGE_PREFIX "Status of pedantic support")

  if (COMPILE_RESULT)
    set (CMAKE_VERBOSE_C_FLAGS "${CMAKE_VERBOSE_C_FLAGS} ${FLAG}")
    message (STATUS "${MESSAGE_PREFIX} - ${FLAG}")
  else ()
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  # -- Wall --

  if (MSVC)
    set (FLAG "/Wall")
  else ()
    set (FLAG "-Wall")
  endif ()

  cmake_test_verbose_flag (${FLAG})

  set (MESSAGE_PREFIX "Status of Wall support")

  if (COMPILE_RESULT)
    set (CMAKE_VERBOSE_C_FLAGS "${CMAKE_VERBOSE_C_FLAGS} ${FLAG}")
    message (STATUS "${MESSAGE_PREFIX} - ${FLAG}")
  else ()
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  # -- Wextra --

  if (MSVC)
    set (FLAG "/W4")
  else ()
    set (FLAG "-Wextra")
  endif ()

  cmake_test_verbose_flag (${FLAG})

  set (MESSAGE_PREFIX "Status of Wextra support")

  if (COMPILE_RESULT)
    set (CMAKE_VERBOSE_C_FLAGS "${CMAKE_VERBOSE_C_FLAGS} ${FLAG}")
    message (STATUS "${MESSAGE_PREFIX} - ${FLAG}")
  else ()
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  set (CMAKE_VERBOSE_C_FLAGS ${CMAKE_VERBOSE_C_FLAGS} CACHE STRING "Verbose C flags")

  # -- result --

  set (CMAKE_GET_VERBOSE_FLAGS_PROCESSED true CACHE STRING "Verbose flags processed")

  mark_as_advanced (
    CMAKE_WERROR_C_FLAGS
    CMAKE_VERBOSE_C_FLAGS
    CMAKE_GET_VERBOSE_FLAGS_PROCESSED
  )
endfunction ()
