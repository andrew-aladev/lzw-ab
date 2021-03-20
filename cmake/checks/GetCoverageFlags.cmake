set (CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

function (cmake_test_coverage FLAG)
  set (NAME "cmake_get_coverage_flags")
  set (SOURCE_DIR "${CURRENT_LIST_DIR}/basic")
  set (BINARY_DIR "${PROJECT_BINARY_DIR}/get_coverage_flags")

  include (GetVerboseFlags)
  cmake_get_verbose_flags ()

  include (CheckRunnable)
  cmake_check_runnable ()

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

  set (COMPILE_RESULT ${COMPILE_RESULT} PARENT_SCOPE)
endfunction ()

function (cmake_get_coverage_flags)
  if (DEFINED CMAKE_GET_COVERAGE_FLAGS_PROCESSED)
    return ()
  endif ()

  set (MESSAGE_PREFIX "Status of coverage support")

  if (NOT MSVC)
    set (FLAG "--coverage")

    cmake_test_coverage (${FLAG})

    if (COMPILE_RESULT)
      set (CMAKE_COVERAGE_C_FLAGS ${FLAG})
      message (STATUS "${MESSAGE_PREFIX} - ${FLAG}")
    else ()
      set (CMAKE_COVERAGE_C_FLAGS "")
      message (STATUS "${MESSAGE_PREFIX} - no")
    endif ()

  else ()
    set (CMAKE_COVERAGE_C_FLAGS "")
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  set (CMAKE_COVERAGE_C_FLAGS ${CMAKE_COVERAGE_C_FLAGS} CACHE STRING "Coverage C flags")
  set (CMAKE_GET_COVERAGE_FLAGS_PROCESSED true CACHE STRING "Coverage flags processed")

  mark_as_advanced (CMAKE_COVERAGE_C_FLAGS CMAKE_GET_COVERAGE_FLAGS_PROCESSED)
endfunction ()
