set (CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

function (cmake_check_tommath)
  if (DEFINED CMAKE_TOMMATH_WORKS)
    return ()
  endif ()

  set (MESSAGE_PREFIX "Status of TomMath support")

  set (NAME "cmake_check_tommath")
  set (SOURCE_DIR "${CURRENT_LIST_DIR}/TomMath")
  set (BINARY_DIR "${PROJECT_BINARY_DIR}/check_tommath")

  find_package (TomMath REQUIRED)

  include (GetVerboseFlags)
  cmake_get_verbose_flags ()

  include (CheckC18)
  cmake_check_c18 ()

  include (CheckRunnable)
  cmake_check_runnable ()

  try_compile (
    COMPILE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
    CMAKE_FLAGS
      "-DCMAKE_C_FLAGS=${CMAKE_VERBOSE_C_FLAGS} ${CMAKE_C18_C_FLAGS} ${CMAKE_WERROR_C_FLAGS}"
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
    message (FATAL_ERROR "${MESSAGE_PREFIX} - not working")
  endif ()

  set (CMAKE_TOMMATH_WORKS ${CMAKE_TOMMATH_WORKS} CACHE STRING "Status of TomMath")

  mark_as_advanced (CMAKE_TOMMATH_WORKS)
endfunction ()
