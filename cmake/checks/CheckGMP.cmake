set (CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

function (cmake_check_gmp)
  if (DEFINED CMAKE_GMP_WORKS)
    return ()
  endif ()

  set (MESSAGE_PREFIX "Status of GMP support")

  set (NAME "cmake_check_gmp")
  set (SOURCE_DIR "${CURRENT_LIST_DIR}/GMP")
  set (BINARY_DIR "${PROJECT_BINARY_DIR}/check_gmp")

  find_package (GMP)

  if (CMAKE_GMP_FOUND)
  endif ()

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
      "-DCMAKE_GMP_INCLUDE_PATH=${CMAKE_GMP_INCLUDE_PATH}"
      "-DCMAKE_GMP_SHARED_LIBRARY_PATH=${CMAKE_GMP_SHARED_LIBRARY_PATH}"
      "-DCMAKE_GMP_STATIC_LIBRARY_PATH=${CMAKE_GMP_STATIC_LIBRARY_PATH}"
      "-DCMAKE_TRY_RUN=${CMAKE_CAN_RUN_EXE}"
    OUTPUT_VARIABLE COMPILE_OUTPUT
  )
  file (REMOVE_RECURSE ${BINARY_DIR})

  if (CMAKE_VERBOSE_MAKEFILE)
    message (STATUS ${COMPILE_OUTPUT})
  endif ()

  if (COMPILE_RESULT)
    set (CMAKE_GMP_WORKS true)
    message (STATUS "${MESSAGE_PREFIX} - working")
  else ()
    set (CMAKE_GMP_WORKS false)
    message (FATAL_ERROR "${MESSAGE_PREFIX} - not working")
  endif ()

  set (CMAKE_GMP_WORKS ${CMAKE_GMP_WORKS} CACHE STRING "Status of GMP")

  mark_as_advanced (CMAKE_GMP_WORKS)
endfunction ()
