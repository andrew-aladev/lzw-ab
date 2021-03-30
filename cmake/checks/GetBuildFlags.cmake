set (CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

function (cmake_test_build_flag FLAG)
  set (NAME "cmake_test_build_flag")
  set (SOURCE_DIR "${CURRENT_LIST_DIR}/basic")
  set (BINARY_DIR "${PROJECT_BINARY_DIR}/test_build_flag")

  include (GetVerboseFlags)
  cmake_get_verbose_flags ()

  include (CheckRunnable)
  cmake_check_runnable ()

  try_compile (
    TEST_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
    CMAKE_FLAGS
      "-DCMAKE_C_FLAGS=${CMAKE_WERROR_C_FLAGS} ${FLAG}"
      "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}"
      "-DCMAKE_TRY_RUN=${CMAKE_CAN_RUN_EXE}"
    OUTPUT_VARIABLE TEST_OUTPUT
  )
  file (REMOVE_RECURSE ${BINARY_DIR})

  if (CMAKE_VERBOSE_MAKEFILE)
    message (STATUS ${TEST_OUTPUT})
  endif ()

  set (TEST_RESULT ${TEST_RESULT} PARENT_SCOPE)
endfunction ()

function (cmake_get_build_flags)
  if (DEFINED CMAKE_GET_BUILD_FLAGS_PROCESSED)
    return ()
  endif ()

  # -- Debug info --

  if (MSVC)
    set (FLAG "/Zi")
  else ()
    set (FLAG "-g")
  endif ()

  cmake_test_build_flag (${FLAG})

  set (MESSAGE_PREFIX "Status of debug info support")

  if (TEST_RESULT)
    set (CMAKE_DEBUG_INFO_C_FLAGS ${FLAG})
    message (STATUS "${MESSAGE_PREFIX} - ${FLAG}")
  else ()
    set (CMAKE_DEBUG_INFO_C_FLAGS "")
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  set (
    CMAKE_DEBUG_INFO_C_FLAGS
    ${CMAKE_DEBUG_INFO_C_FLAGS}
    CACHE STRING "Debug info C flags"
  )

  # -- No optimizations --

  if (MSVC)
    set (FLAG "/Od")
  else ()
    set (FLAG "-O0")
  endif ()

  cmake_test_build_flag (${FLAG})

  set (MESSAGE_PREFIX "Status of no optimizations support")

  if (TEST_RESULT)
    set (CMAKE_NO_OPTIMIZATIONS_C_FLAGS ${FLAG})
    message (STATUS "${MESSAGE_PREFIX} - ${FLAG}")
  else ()
    set (CMAKE_NO_OPTIMIZATIONS_C_FLAGS "")
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  set (
    CMAKE_NO_OPTIMIZATIONS_C_FLAGS
    ${CMAKE_NO_OPTIMIZATIONS_C_FLAGS}
    CACHE STRING "No optimizations C flags"
  )

  # -- Optimizations --

  if (MSVC)
    set (FLAG "/O2")
  else ()
    set (FLAG "-O2")
  endif ()

  cmake_test_build_flag (${FLAG})

  set (MESSAGE_PREFIX "Status of optimizations support")

  if (TEST_RESULT)
    set (CMAKE_OPTIMIZATIONS_C_FLAGS ${FLAG})
    message (STATUS "${MESSAGE_PREFIX} - ${FLAG}")
  else ()
    set (CMAKE_OPTIMIZATIONS_C_FLAGS "")
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  set (
    CMAKE_OPTIMIZATIONS_C_FLAGS
    ${CMAKE_OPTIMIZATIONS_C_FLAGS}
    CACHE STRING "Optimizations C flags"
  )

  # -- Min size optimizations --

  if (MSVC)
    set (FLAG "/Os")
  else ()
    set (FLAG "-Os")
  endif ()

  cmake_test_build_flag (${FLAG})

  set (MESSAGE_PREFIX "Status of min size optimizations support")

  if (TEST_RESULT)
    set (CMAKE_MIN_SIZE_OPTIMIZATIONS_C_FLAGS ${FLAG})
    message (STATUS "${MESSAGE_PREFIX} - ${FLAG}")
  else ()
    set (CMAKE_MIN_SIZE_OPTIMIZATIONS_C_FLAGS "")
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  set (
    CMAKE_MIN_SIZE_OPTIMIZATIONS_C_FLAGS
    ${CMAKE_MIN_SIZE_OPTIMIZATIONS_C_FLAGS}
    CACHE STRING "Min size optimizations C flags"
  )

  # -- pipe --

  set (MESSAGE_PREFIX "Status of pipe support")

  if (NOT MSVC)
    set (FLAG "-pipe")

    cmake_test_build_flag (${FLAG})

    if (TEST_RESULT)
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

  set (
    CMAKE_PIPE_C_FLAGS
    ${CMAKE_PIPE_C_FLAGS}
    CACHE STRING "Pipe C flags"
  )

  # -- result --

  set (
    CMAKE_C_FLAGS_DEBUG
    "${CMAKE_NO_OPTIMIZATIONS_C_FLAGS} ${CMAKE_DEBUG_INFO_C_FLAGS}"
    CACHE STRING "Debug C flags" FORCE
  )
  set (
    CMAKE_C_FLAGS_RELEASE
    ${CMAKE_OPTIMIZATIONS_C_FLAGS}
    CACHE STRING "Release C flags" FORCE
  )
  set (
    CMAKE_C_FLAGS_RELWITHDEBINFO
    "${CMAKE_OPTIMIZATIONS_C_FLAGS} ${CMAKE_DEBUG_INFO_C_FLAGS}"
    CACHE STRING "RelWithDebInfo C flags" FORCE
  )
  set (
    CMAKE_C_FLAGS_MINSIZEREL
    ${CMAKE_NO_OPTIMIZATIONS_C_FLAGS}
    CACHE STRING "MinSizeRel C flags" FORCE
  )

  set (CMAKE_GET_BUILD_FLAGS_PROCESSED true CACHE BOOL "Build flags processed")

  mark_as_advanced (
    CMAKE_DEBUG_INFO_C_FLAGS
    CMAKE_NO_OPTIMIZATIONS_C_FLAGS
    CMAKE_OPTIMIZATIONS_C_FLAGS
    CMAKE_MIN_SIZE_OPTIMIZATIONS_C_FLAGS
    CMAKE_PIPE_C_FLAGS
    CMAKE_C_FLAGS_DEBUG
    CMAKE_C_FLAGS_RELEASE
    CMAKE_C_FLAGS_RELWITHDEBINFO
    CMAKE_C_FLAGS_MINSIZEREL
    CMAKE_GET_BUILD_FLAGS_PROCESSED
  )
endfunction ()
