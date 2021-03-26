set (CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

function (cmake_test_export_mode EXPORT_MODE_UPPERCASE)
  set (NAME "cmake_test_export_mode")
  set (SOURCE_DIR "${CURRENT_LIST_DIR}/export")
  set (BINARY_DIR "${PROJECT_BINARY_DIR}/test_export_mode")

  include (GetVerboseFlags)
  cmake_get_verbose_flags ()

  include (CheckC17)
  cmake_check_c17 ()

  include (CheckRunnable)
  cmake_check_runnable ()

  try_compile (
    TEST_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
    CMAKE_FLAGS
      "-DCMAKE_C_FLAGS=${CMAKE_VERBOSE_C_FLAGS} ${CMAKE_C17_C_FLAGS}"
      "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}"
      "-DCMAKE_EXPORT_MODE_UPPERCASE=${EXPORT_MODE_UPPERCASE}"
      "-DCMAKE_TRY_RUN=${CMAKE_CAN_RUN_EXE}"
    OUTPUT_VARIABLE TEST_OUTPUT
  )
  file (REMOVE_RECURSE ${BINARY_DIR})

  if (CMAKE_VERBOSE_MAKEFILE)
    message (STATUS ${TEST_OUTPUT})
  endif ()

  set (TEST_RESULT ${TEST_RESULT} PARENT_SCOPE)
endfunction ()

function (cmake_get_export_mode CHECK_MODE)
  if (DEFINED CMAKE_EXPORT_MODE)
    return ()
  endif ()

  if (WIN32 OR CYGWIN)
    set (EXPORT_MODES "dll" "vanilla")
  else ()
    set (EXPORT_MODES "visibility" "vanilla")
  endif ()

  set (MESSAGE_PREFIX "Status of export mode support")

  foreach (EXPORT_MODE ${EXPORT_MODES})
    string (TOUPPER ${EXPORT_MODE} EXPORT_MODE_UPPERCASE)

    cmake_test_export_mode (${EXPORT_MODE_UPPERCASE})

    if (TEST_RESULT)
      set (CMAKE_EXPORT_MODE ${EXPORT_MODE})
      message (STATUS "${MESSAGE_PREFIX} - ${EXPORT_MODE}")
      break ()
    endif ()
  endforeach ()

  if (NOT TEST_RESULT)
    set (CMAKE_EXPORT_MODE "")
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  string (TOUPPER ${CMAKE_EXPORT_MODE} CMAKE_EXPORT_MODE_UPPERCASE)

  set (
    CMAKE_EXPORT_MODE ${CMAKE_EXPORT_MODE}
    CACHE STRING "Export mode"
  )
  set (
    CMAKE_EXPORT_MODE_UPPERCASE ${CMAKE_EXPORT_MODE_UPPERCASE}
    CACHE STRING "Export mode uppercase"
  )

  mark_as_advanced (CMAKE_EXPORT_MODE CMAKE_EXPORT_MODE_UPPERCASE)

  if (CMAKE_EXPORT_MODE STREQUAL "" AND CHECK_MODE STREQUAL "REQUIRED")
    message (FATAL_ERROR "${MESSAGE_PREFIX} - export mode is required")
  endif ()
endfunction ()
