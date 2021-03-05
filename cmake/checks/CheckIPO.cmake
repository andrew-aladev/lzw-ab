set (CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

function (cmake_check_ipo)
  if (DEFINED CMAKE_INTERPROCEDURAL_OPTIMIZATION)
    return ()
  endif ()

  set (MESSAGE_PREFIX "Status of IPO support")

  set (NAME "cmake_check_ipo")
  set (SOURCE_DIR "${CURRENT_LIST_DIR}/basic")
  set (BINARY_DIR "${PROJECT_BINARY_DIR}/check_ipo")

  include (GetVerboseFlags)
  cmake_get_verbose_flags ()

  include (CheckC18)
  cmake_check_c18 ()

  include (CheckRunnable)
  cmake_check_runnable ()

  include (CheckIPOSupported)
  check_ipo_supported (RESULT CHECK_RESULT OUTPUT CHECK_OUTPUT)

  if (CMAKE_VERBOSE_MAKEFILE)
    message (STATUS ${CHECK_OUTPUT})
  endif ()

  if (CHECK_RESULT)
    try_compile (
      COMPILE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
      CMAKE_FLAGS
        "-DCMAKE_C_FLAGS=${CMAKE_VERBOSE_C_FLAGS} ${CMAKE_C18_C_FLAGS} ${CMAKE_WERROR_C_FLAGS}"
        "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}"
        "-DCMAKE_TRY_RUN=${CMAKE_CAN_RUN_EXE}"
        "-DCMAKE_INTERPROCEDURAL_OPTIMIZATION=1"
      OUTPUT_VARIABLE COMPILE_OUTPUT
    )
    file (REMOVE_RECURSE ${BINARY_DIR})

    if (CMAKE_VERBOSE_MAKEFILE)
      message (STATUS ${COMPILE_OUTPUT})
    endif ()

    if (COMPILE_RESULT)
      set (CMAKE_INTERPROCEDURAL_OPTIMIZATION true)
      message (STATUS "${MESSAGE_PREFIX} - working")
    else ()
      set (CMAKE_INTERPROCEDURAL_OPTIMIZATION false)
      message (STATUS "${MESSAGE_PREFIX} - not working")
    endif ()

  else ()
    set (CMAKE_INTERPROCEDURAL_OPTIMIZATION false)
    message (STATUS "${MESSAGE_PREFIX} - not working")
  endif ()

  set (
    CMAKE_INTERPROCEDURAL_OPTIMIZATION
    ${CMAKE_INTERPROCEDURAL_OPTIMIZATION}
    CACHE BOOL "Status of IPO"
  )

  mark_as_advanced (CMAKE_INTERPROCEDURAL_OPTIMIZATION)
endfunction ()
