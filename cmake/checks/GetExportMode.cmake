set (CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

function (cmake_test_export_mode MODE)
  set (NAME "cmake_test_export_mode")
  set (SOURCE_DIR "${CURRENT_LIST_DIR}/export_mode")
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
      "-DCMAKE_EXPORT_MODE=${MODE}"
      "-DCMAKE_TRY_RUN=${CMAKE_CAN_RUN_EXE}"
    OUTPUT_VARIABLE TEST_OUTPUT
  )
  file (REMOVE_RECURSE ${BINARY_DIR})

  if (CMAKE_VERBOSE_MAKEFILE)
    message (STATUS ${TEST_OUTPUT})
  endif ()

  set (TEST_RESULT ${TEST_RESULT} PARENT_SCOPE)
endfunction ()

function (cmake_get_export_mode)
  if (DEFINED CMAKE_EXPORT_MODE)
    return ()
  endif ()

  if (WIN32 OR CYGWIN)
    set (MODES "dll" "vanilla")
  else ()
    set (MODES "visibility")
  endif ()

  set (MESSAGE_PREFIX "Status of export mode support")

  foreach (MODE ${MODES})
    cmake_test_export_mode (${MODE})

    if (TEST_RESULT)
      set (CMAKE_EXPORT_MODE ${MODE})
      message (STATUS "${MESSAGE_PREFIX} - ${MODE}")
      break ()
    endif ()
  endforeach ()

  if (NOT TEST_RESULT)
    set (CMAKE_EXPORT_MODE "")
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  set (CMAKE_EXPORT_MODE ${CMAKE_EXPORT_MODE} CACHE STRING "Export mode")

  mark_as_advanced (CMAKE_EXPORT_MODE)
endfunction ()
