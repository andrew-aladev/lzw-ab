set (CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

function (cmake_test_gmp)
  set (NAME "cmake_test_gmp")
  set (SOURCE_DIR "${CURRENT_LIST_DIR}/GMP")
  set (BINARY_DIR "${PROJECT_BINARY_DIR}/test_gmp")

  include (GetVerboseFlags)
  cmake_get_verbose_flags ()

  include (CheckC11AndAbove)
  cmake_check_c11_and_above (REQUIRED)

  include (CheckRunnable)
  cmake_check_runnable ()

  try_compile (
    TEST_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
    CMAKE_FLAGS
      "-DCMAKE_C_FLAGS=${CMAKE_VERBOSE_C_FLAGS} ${CMAKE_C11_AND_ABOVE_C_FLAGS}"
      "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}"
      "-DCMAKE_GMP_INCLUDE_PATH=${CMAKE_GMP_INCLUDE_PATH}"
      "-DCMAKE_GMP_SHARED_LIBRARY_PATH=${CMAKE_GMP_SHARED_LIBRARY_PATH}"
      "-DCMAKE_GMP_STATIC_LIBRARY_PATH=${CMAKE_GMP_STATIC_LIBRARY_PATH}"
      "-DCMAKE_TRY_RUN=${CMAKE_CAN_RUN_EXE}"
    OUTPUT_VARIABLE TEST_OUTPUT
  )
  file (REMOVE_RECURSE ${BINARY_DIR})

  if (CMAKE_VERBOSE_MAKEFILE)
    message (STATUS ${TEST_OUTPUT})
  endif ()

  set (TEST_RESULT ${TEST_RESULT} PARENT_SCOPE)
endfunction ()

function (cmake_check_gmp CHECK_MODE)
  set (MESSAGE_PREFIX "Status of GMP support")

  if (DEFINED CMAKE_GMP_WORKS)
    if (NOT CMAKE_GMP_WORKS AND CHECK_MODE STREQUAL "REQUIRED")
      message (FATAL_ERROR "${MESSAGE_PREFIX} - is required")
    endif ()

    return ()
  endif ()

  find_package (GMP)

  if (CMAKE_GMP_FOUND)
    cmake_test_gmp ()

    if (TEST_RESULT)
      set (CMAKE_GMP_WORKS true)
      message (STATUS "${MESSAGE_PREFIX} - working")
    else ()
      set (CMAKE_GMP_WORKS false)
      message (STATUS "${MESSAGE_PREFIX} - not working")
    endif ()

  else ()
    set (CMAKE_GMP_WORKS false)
    message (STATUS "${MESSAGE_PREFIX} - not working")
  endif ()

  set (
    CMAKE_GMP_WORKS ${CMAKE_GMP_WORKS}
    CACHE BOOL "Status of GMP"
  )

  mark_as_advanced (CMAKE_GMP_WORKS)

  if (NOT CMAKE_GMP_WORKS AND CHECK_MODE STREQUAL "REQUIRED")
    message (FATAL_ERROR "${MESSAGE_PREFIX} - is required")
  endif ()
endfunction ()
