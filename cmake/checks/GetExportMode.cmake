set (CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

function (cmake_test_export_mode EXPORT_MODE WINDOWS_EXPORT_ALL_SYMBOLS)
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
      "-DCMAKE_EXPORT_MODE=${EXPORT_MODE}"
      "-DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=${WINDOWS_EXPORT_ALL_SYMBOLS}"
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
    set (IS_WINDOWS true)
  else ()
    set (IS_WINDOWS false)
  endif ()

  if (IS_WINDOWS)
    set (EXPORT_MODES "DLL" "DLL_AUTO")
  else ()
    set (EXPORT_MODES "VISIBILITY")
  endif ()

  set (MESSAGE_PREFIX "Status of export mode support")

  foreach (EXPORT_MODE ${EXPORT_MODES})
    if (IS_WINDOWS AND EXPORT_MODE STREQUAL "DLL_AUTO")
      set (WINDOWS_EXPORT_ALL_SYMBOLS true)
    else ()
      set (WINDOWS_EXPORT_ALL_SYMBOLS false)
    endif ()

    cmake_test_export_mode (${EXPORT_MODE} ${WINDOWS_EXPORT_ALL_SYMBOLS})

    if (TEST_RESULT)
      set (CMAKE_EXPORT_MODE ${EXPORT_MODE})
      set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ${WINDOWS_EXPORT_ALL_SYMBOLS})

      if (IS_WINDOWS)
        if (WINDOWS_EXPORT_ALL_SYMBOLS)
          set (EXPORT_ALL_SYMBOLS_STATUS "enabled")
        else ()
          set (EXPORT_ALL_SYMBOLS_STATUS "disabled")
        endif ()

        message (
          STATUS
          "${MESSAGE_PREFIX} - ${EXPORT_MODE}, export all symbols - ${EXPORT_ALL_SYMBOLS_STATUS}"
        )
      else ()
        message (STATUS "${MESSAGE_PREFIX} - ${EXPORT_MODE}")
      endif ()

      break ()
    endif ()
  endforeach ()

  if (NOT TEST_RESULT)
    set (CMAKE_EXPORT_MODE "")
    set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS false)

    if (IS_WINDOWS)
      message (STATUS "${MESSAGE_PREFIX} - no, export all symbols - disabled")
    else ()
      message (STATUS "${MESSAGE_PREFIX} - no")
    endif ()
  endif ()

  set (CMAKE_EXPORT_MODE ${CMAKE_EXPORT_MODE} CACHE STRING "Export mode")
  set (
    CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ${CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS}
    CACHE STRING "Export all symbols for windows"
  )

  mark_as_advanced (CMAKE_EXPORT_MODE CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS)
endfunction ()
