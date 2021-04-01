set (CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

function (cmake_test_coverage FLAG)
  set (NAME "cmake_test_coverage")
  set (SOURCE_DIR "${CURRENT_LIST_DIR}/basic")
  set (BINARY_DIR "${PROJECT_BINARY_DIR}/test_coverage")

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

function (cmake_get_coverage_flags)
  if (DEFINED CMAKE_HAVE_COVERAGE)
    return ()
  endif ()

  set (MESSAGE_PREFIX "Status of coverage support")

  if (NOT MSVC)
    set (FLAG "--coverage")

    cmake_test_coverage (${FLAG})

    if (TEST_RESULT)
      set (CMAKE_HAVE_COVERAGE true)
      set (CMAKE_COVERAGE_C_FLAGS ${FLAG})
      message (STATUS "${MESSAGE_PREFIX} - ${FLAG}")
    else ()
      set (CMAKE_HAVE_COVERAGE false)
      set (CMAKE_COVERAGE_C_FLAGS "")
      message (STATUS "${MESSAGE_PREFIX} - no")
    endif ()

  else ()
    set (CMAKE_HAVE_COVERAGE false)
    set (CMAKE_COVERAGE_C_FLAGS "")
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  set (
    CMAKE_HAVE_COVERAGE ${CMAKE_HAVE_COVERAGE}
    CACHE BOOL "Coverage flags processed"
  )
  set (
    CMAKE_COVERAGE_C_FLAGS ${CMAKE_COVERAGE_C_FLAGS}
    CACHE STRING "Coverage C flags"
  )

  mark_as_advanced (CMAKE_HAVE_COVERAGE CMAKE_COVERAGE_C_FLAGS)
endfunction ()
