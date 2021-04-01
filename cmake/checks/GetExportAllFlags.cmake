set (CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

function (cmake_test_export_all EXPORT_ALL_LD_FLAGS)
  set (NAME "cmake_test_export_all")
  set (SOURCE_DIR "${CURRENT_LIST_DIR}/export")
  set (BINARY_DIR "${PROJECT_BINARY_DIR}/test_export_all")

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
      "-DCMAKE_SHARED_LIBRARY_LD_FLAGS=${EXPORT_ALL_LD_FLAGS}"
      "-DCMAKE_TRY_RUN=${CMAKE_CAN_RUN_EXE}"
    OUTPUT_VARIABLE TEST_OUTPUT
  )
  file (REMOVE_RECURSE ${BINARY_DIR})

  if (CMAKE_VERBOSE_MAKEFILE)
    message (STATUS ${TEST_OUTPUT})
  endif ()

  set (TEST_RESULT ${TEST_RESULT} PARENT_SCOPE)
endfunction ()

function (cmake_get_export_all_flags)
  if (DEFINED CMAKE_GET_EXPORT_ALL_FLAGS_PROCESSED)
    return ()
  endif ()

  set (MESSAGE_PREFIX "Status of export all support")

  if (NOT MSVC)
    set (
      EXPORT_ALL_LD_FLAG_GROUPS
      "-Wl,--export-all-symbols -Wl,--enable-auto-import"
      "-fvisibility=default"
      ""
    )

    foreach (EXPORT_ALL_LD_FLAGS ${EXPORT_ALL_LD_FLAG_GROUPS})
      cmake_test_export_all (${EXPORT_ALL_LD_FLAGS})

      if (TEST_RESULT)
        set (CMAKE_EXPORT_ALL_LD_FLAGS ${EXPORT_ALL_LD_FLAGS})

        if (EXPORT_ALL_LD_FLAGS STREQUAL "")
          set (EXPORT_ALL_LD_FLAGS "vanilla")
        endif ()

        message (STATUS "${MESSAGE_PREFIX} - ${EXPORT_ALL_LD_FLAGS}")
        break ()
      endif ()
    endforeach ()

    if (NOT TEST_RESULT)
      set (CMAKE_EXPORT_ALL_LD_FLAGS "")
      message (STATUS "${MESSAGE_PREFIX} - no")
    endif ()

  else ()
    set (CMAKE_EXPORT_ALL_LD_FLAGS "")
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  set (
    CMAKE_EXPORT_ALL_LD_FLAGS ${CMAKE_EXPORT_ALL_LD_FLAGS}
    CACHE STRING "Export all LD flags"
  )
  set (
    CMAKE_GET_EXPORT_ALL_FLAGS_PROCESSED true
    CACHE BOOL "Export all flags processed"
  )

  mark_as_advanced (
    CMAKE_EXPORT_ALL_LD_FLAGS
    CMAKE_GET_EXPORT_ALL_FLAGS_PROCESSED
  )
endfunction ()
