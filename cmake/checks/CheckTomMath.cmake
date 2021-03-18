set (CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

function (cmake_check_tommath CHECK_MODE)
  if (DEFINED CMAKE_TOMMATH_WORKS)
    return ()
  endif ()

  set (NAME "cmake_check_tommath")
  set (SOURCE_DIR "${CURRENT_LIST_DIR}/TomMath")
  set (BINARY_DIR "${PROJECT_BINARY_DIR}/check_tommath")

  include (CheckC17)
  cmake_check_c17 ()

  include (CheckRunnable)
  cmake_check_runnable ()

  find_package (TomMath)

  set (MESSAGE_PREFIX "Status of TomMath support")

  if (CMAKE_TOMMATH_FOUND)
    try_compile (
      COMPILE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
      CMAKE_FLAGS
        "-DCMAKE_C_FLAGS=${CMAKE_C17_C_FLAGS}"
        "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}"
        "-DCMAKE_TOMMATH_INCLUDE_PATH=${CMAKE_TOMMATH_INCLUDE_PATH}"
        "-DCMAKE_TOMMATH_SHARED_LIBRARY_PATH=${CMAKE_TOMMATH_SHARED_LIBRARY_PATH}"
        "-DCMAKE_TOMMATH_STATIC_LIBRARY_PATH=${CMAKE_TOMMATH_STATIC_LIBRARY_PATH}"
        "-DCMAKE_TRY_RUN=${CMAKE_CAN_RUN_EXE}"
      OUTPUT_VARIABLE COMPILE_OUTPUT
    )
    file (REMOVE_RECURSE ${BINARY_DIR})

    if (CMAKE_VERBOSE_MAKEFILE)
      message (STATUS ${COMPILE_OUTPUT})
    endif ()

    if (COMPILE_RESULT)
      set (CMAKE_TOMMATH_WORKS true)
      message (STATUS "${MESSAGE_PREFIX} - working")
    else ()
      set (CMAKE_TOMMATH_WORKS false)
      message (STATUS "${MESSAGE_PREFIX} - not working")
    endif ()

  else ()
    set (CMAKE_TOMMATH_WORKS false)
    message (STATUS "${MESSAGE_PREFIX} - not working")
  endif ()

  set (CMAKE_TOMMATH_WORKS ${CMAKE_TOMMATH_WORKS} CACHE STRING "Status of TomMath")

  mark_as_advanced (CMAKE_TOMMATH_WORKS)

  if (NOT CMAKE_TOMMATH_WORKS AND CHECK_MODE STREQUAL "REQUIRED")
    message (FATAL_ERROR "${MESSAGE_PREFIX} - TomMath is required")
  endif ()
endfunction ()
