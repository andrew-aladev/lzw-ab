set (CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

function (cmake_test_verbose_flag FLAG)
  set (NAME "cmake_test_verbose_flag")
  set (SOURCE_DIR "${CURRENT_LIST_DIR}/basic")
  set (BINARY_DIR "${PROJECT_BINARY_DIR}/test_verbose_flag")

  try_compile (
    TEST_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
    CMAKE_FLAGS
      "-DCMAKE_C_FLAGS=${FLAG}"
      "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}"
    OUTPUT_VARIABLE TEST_OUTPUT
  )
  file (REMOVE_RECURSE ${BINARY_DIR})

  if (CMAKE_VERBOSE_MAKEFILE)
    message (STATUS ${TEST_OUTPUT})
  endif ()

  set (TEST_RESULT ${TEST_RESULT} PARENT_SCOPE)
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

  if (TEST_RESULT)
    set (CMAKE_WERROR_C_FLAGS ${FLAG})
    message (STATUS "${MESSAGE_PREFIX} - ${FLAG}")
  else ()
    set (CMAKE_WERROR_C_FLAGS "")
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  # -- pedantic --

  if (MSVC)
    set (FLAG "/permissive-")
  else ()
    set (FLAG "-pedantic")
  endif ()

  set (CMAKE_VERBOSE_C_FLAGS "")

  cmake_test_verbose_flag (${FLAG})

  set (MESSAGE_PREFIX "Status of pedantic support")

  if (TEST_RESULT)
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

  if (TEST_RESULT)
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

  if (TEST_RESULT)
    set (CMAKE_VERBOSE_C_FLAGS "${CMAKE_VERBOSE_C_FLAGS} ${FLAG}")
    message (STATUS "${MESSAGE_PREFIX} - ${FLAG}")
  else ()
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  # -- Wconversion --

  set (MESSAGE_PREFIX "Status of Wconversion support")

  if (NOT MSVC)
    set (FLAG "-Wconversion")

    cmake_test_verbose_flag (${FLAG})

    if (TEST_RESULT)
      set (CMAKE_VERBOSE_C_FLAGS "${CMAKE_VERBOSE_C_FLAGS} ${FLAG}")
      message (STATUS "${MESSAGE_PREFIX} - ${FLAG}")
    else ()
      message (STATUS "${MESSAGE_PREFIX} - no")
    endif ()
  else ()
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  # -- result --

  set (
    CMAKE_WERROR_C_FLAGS ${CMAKE_WERROR_C_FLAGS}
    CACHE STRING "Werror C flags"
  )
  set (
    CMAKE_VERBOSE_C_FLAGS ${CMAKE_VERBOSE_C_FLAGS}
    CACHE STRING "Verbose C flags"
  )
  set (
    CMAKE_GET_VERBOSE_FLAGS_PROCESSED true
    CACHE BOOL "Verbose flags processed"
  )

  mark_as_advanced (
    CMAKE_WERROR_C_FLAGS
    CMAKE_VERBOSE_C_FLAGS
    CMAKE_GET_VERBOSE_FLAGS_PROCESSED
  )
endfunction ()
