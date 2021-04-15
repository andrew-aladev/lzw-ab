set (CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

function (cmake_test_popt)
  set (NAME "cmake_test_popt")
  set (SOURCE_DIR "${CURRENT_LIST_DIR}/Popt")
  set (BINARY_DIR "${PROJECT_BINARY_DIR}/test_popt")

  include (GetVerboseFlags)
  cmake_get_verbose_flags ()

  include (CheckC17)
  cmake_check_c17 (REQUIRED)

  include (CheckRunnable)
  cmake_check_runnable ()

  try_compile (
    TEST_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
    CMAKE_FLAGS
      "-DCMAKE_C_FLAGS=${CMAKE_VERBOSE_C_FLAGS} ${CMAKE_C17_C_FLAGS}"
      "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}"
      "-DCMAKE_POPT_INCLUDE_PATH=${CMAKE_POPT_INCLUDE_PATH}"
      "-DCMAKE_POPT_SHARED_LIBRARY_PATH=${CMAKE_POPT_SHARED_LIBRARY_PATH}"
      "-DCMAKE_POPT_STATIC_LIBRARY_PATH=${CMAKE_POPT_STATIC_LIBRARY_PATH}"
      "-DCMAKE_TRY_RUN=${CMAKE_CAN_RUN_EXE}"
    OUTPUT_VARIABLE TEST_OUTPUT
  )
  file (REMOVE_RECURSE ${BINARY_DIR})

  if (CMAKE_VERBOSE_MAKEFILE)
    message (STATUS ${TEST_OUTPUT})
  endif ()

  set (TEST_RESULT ${TEST_RESULT} PARENT_SCOPE)
endfunction ()

function (cmake_check_popt CHECK_MODE)
  set (MESSAGE_PREFIX "Status of Popt support")

  if (DEFINED CMAKE_POPT_WORKS)
    if (NOT CMAKE_POPT_WORKS AND CHECK_MODE STREQUAL "REQUIRED")
      message (FATAL_ERROR "${MESSAGE_PREFIX} - is required")
    endif ()

    return ()
  endif ()

  find_package (Popt)

  if (CMAKE_POPT_FOUND)
    cmake_test_popt ()

    if (TEST_RESULT)
      set (CMAKE_POPT_WORKS true)
      message (STATUS "${MESSAGE_PREFIX} - working")
    else ()
      set (CMAKE_POPT_WORKS false)
      message (STATUS "${MESSAGE_PREFIX} - not working")
    endif ()

  else ()
    set (CMAKE_POPT_WORKS false)
    message (STATUS "${MESSAGE_PREFIX} - not working")
  endif ()

  set (
    CMAKE_POPT_WORKS ${CMAKE_POPT_WORKS}
    CACHE BOOL "Status of Popt"
  )

  mark_as_advanced (CMAKE_POPT_WORKS)

  if (NOT CMAKE_POPT_WORKS AND CHECK_MODE STREQUAL "REQUIRED")
    message (FATAL_ERROR "${MESSAGE_PREFIX} - is required")
  endif ()
endfunction ()
