set (CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

function (
  cmake_test_export_mode_with_ld_flags
  EXPORT_MODE_UPPERCASE
  EXPORT_SHARED_LIBRARY_LD_FLAGS
  EXPORT_EXECUTABLE_LD_FLAGS
)
  set (NAME "cmake_test_export_mode_with_ld_flags")
  set (SOURCE_DIR "${CURRENT_LIST_DIR}/export")
  set (BINARY_DIR "${PROJECT_BINARY_DIR}/test_export_mode_with_ld_flags")

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
      "-DCMAKE_EXPORT_MODE_UPPERCASE=${EXPORT_MODE_UPPERCASE}"
      "-DCMAKE_SHARED_LIBRARY_LD_FLAGS=${EXPORT_SHARED_LIBRARY_LD_FLAGS}"
      "-DCMAKE_EXECUTABLE_LD_FLAGS=${EXPORT_EXECUTABLE_LD_FLAGS}"
      "-DCMAKE_TRY_RUN=${CMAKE_CAN_RUN_EXE}"
    OUTPUT_VARIABLE TEST_OUTPUT
  )
  file (REMOVE_RECURSE ${BINARY_DIR})

  if (CMAKE_VERBOSE_MAKEFILE)
    message (STATUS ${TEST_OUTPUT})
  endif ()

  set (TEST_RESULT ${TEST_RESULT} PARENT_SCOPE)
endfunction ()

function (
  cmake_test_export_mode_with_ld_flag_groups
  EXPORT_MODE_UPPERCASE
  EXPORT_ALL_LD_FLAG_GROUPS
  IMPORT_ALL_LD_FLAG_GROUPS
)
  list (LENGTH EXPORT_ALL_LD_FLAG_GROUPS EXPORT_ALL_LD_FLAG_COUNT)
  math (EXPR EXPORT_ALL_LD_FLAG_COUNT "${EXPORT_ALL_LD_FLAG_COUNT}-1")

  foreach (index RANGE ${EXPORT_ALL_LD_FLAG_COUNT})
    list (
      GET EXPORT_ALL_LD_FLAG_GROUPS
      ${index} EXPORT_SHARED_LIBRARY_LD_FLAGS
    )
    if (EXPORT_SHARED_LIBRARY_LD_FLAGS STREQUAL "vanilla")
      set (EXPORT_SHARED_LIBRARY_LD_FLAGS "")
    endif ()

    list (
      GET IMPORT_ALL_LD_FLAG_GROUPS
      ${index} EXPORT_EXECUTABLE_LD_FLAGS
    )
    if (EXPORT_EXECUTABLE_LD_FLAGS STREQUAL "vanilla")
      set (EXPORT_EXECUTABLE_LD_FLAGS "")
    endif ()

    cmake_test_export_mode_with_ld_flags (
      "${EXPORT_MODE_UPPERCASE}"
      "${EXPORT_SHARED_LIBRARY_LD_FLAGS}"
      "${EXPORT_EXECUTABLE_LD_FLAGS}"
    )

    if (TEST_RESULT)
      break ()
    endif ()
  endforeach ()

  if (NOT TEST_RESULT)
    set (EXPORT_SHARED_LIBRARY_LD_FLAGS "")
    set (EXPORT_EXECUTABLE_LD_FLAGS "")
  endif ()

  set (TEST_RESULT ${TEST_RESULT} PARENT_SCOPE)
  set (
    EXPORT_SHARED_LIBRARY_LD_FLAGS "${EXPORT_SHARED_LIBRARY_LD_FLAGS}"
    PARENT_SCOPE
  )
  set (
    EXPORT_EXECUTABLE_LD_FLAGS "${EXPORT_EXECUTABLE_LD_FLAGS}"
    PARENT_SCOPE
  )
endfunction ()

function (cmake_get_export_mode CHECK_MODE)
  set (MESSAGE_PREFIX "Status of export mode support")

  if (DEFINED CMAKE_HAVE_EXPORT)
    if (NOT CMAKE_HAVE_EXPORT AND CHECK_MODE STREQUAL "REQUIRED")
      message (FATAL_ERROR "${MESSAGE_PREFIX} - is required")
    endif ()

    return ()
  endif ()

  if (WIN32 OR CYGWIN OR MSYS OR MINGW)
    set (EXPORT_MODES "dll" "dllexport" "dllimport" "auto")
  else ()
    set (EXPORT_MODES "visibility" "auto")
  endif ()

  if (WIN32 OR CYGWIN OR MSYS OR MINGW)
    if (MSVC)
      set (DEFAULT_EXPORT_ALL_LD_FLAG_GROUPS "vanilla")
      set (DEFAULT_IMPORT_ALL_LD_FLAG_GROUPS "vanilla")
    else ()
      set (
        DEFAULT_EXPORT_ALL_LD_FLAG_GROUPS
        "-Wl,--export-all-symbols -Wl,--enable-auto-import"
        "vanilla"
      )
      set (
        DEFAULT_IMPORT_ALL_LD_FLAG_GROUPS
        "-Wl,--enable-auto-import"
        "vanilla"
      )
    endif ()
  else ()
    set (
      DEFAULT_EXPORT_ALL_LD_FLAG_GROUPS
      "-fvisibility=default"
      "vanilla"
    )
    set (
      DEFAULT_IMPORT_ALL_LD_FLAG_GROUPS
      "-fvisibility=default"
      "vanilla"
    )
  endif ()

  foreach (EXPORT_MODE ${EXPORT_MODES})
    string (TOUPPER ${EXPORT_MODE} EXPORT_MODE_UPPERCASE)

    if (EXPORT_MODE STREQUAL "dllexport")
      set (EXPORT_ALL_LD_FLAG_GROUPS "vanilla")
      set (IMPORT_ALL_LD_FLAG_GROUPS ${DEFAULT_IMPORT_ALL_LD_FLAG_GROUPS})
    elseif (EXPORT_MODE STREQUAL "dllimport")
      set (EXPORT_ALL_LD_FLAG_GROUPS ${DEFAULT_EXPORT_ALL_LD_FLAG_GROUPS})
      set (IMPORT_ALL_LD_FLAG_GROUPS "vanilla")
    elseif (EXPORT_MODE STREQUAL "auto")
      set (EXPORT_ALL_LD_FLAG_GROUPS ${DEFAULT_EXPORT_ALL_LD_FLAG_GROUPS})
      set (IMPORT_ALL_LD_FLAG_GROUPS ${DEFAULT_IMPORT_ALL_LD_FLAG_GROUPS})
    else ()
      set (EXPORT_ALL_LD_FLAG_GROUPS "vanilla")
      set (IMPORT_ALL_LD_FLAG_GROUPS "vanilla")
    endif ()

    cmake_test_export_mode_with_ld_flag_groups (
      "${EXPORT_MODE_UPPERCASE}"
      "${EXPORT_ALL_LD_FLAG_GROUPS}"
      "${IMPORT_ALL_LD_FLAG_GROUPS}"
    )

    if (TEST_RESULT)
      set (CMAKE_HAVE_EXPORT true)
      set (CMAKE_EXPORT_MODE ${EXPORT_MODE})
      set (CMAKE_EXPORT_MODE_UPPERCASE ${EXPORT_MODE_UPPERCASE})
      set (CMAKE_EXPORT_SHARED_LIBRARY_LD_FLAGS ${EXPORT_SHARED_LIBRARY_LD_FLAGS})
      set (CMAKE_EXPORT_EXECUTABLE_LD_FLAGS ${EXPORT_EXECUTABLE_LD_FLAGS})
      set (MESSAGE_STATUS "${MESSAGE_PREFIX} - ${EXPORT_MODE}")

      if (NOT EXPORT_SHARED_LIBRARY_LD_FLAGS STREQUAL "")
        set (
          MESSAGE_STATUS
          "${MESSAGE_STATUS}, shared library LD flags: ${EXPORT_SHARED_LIBRARY_LD_FLAGS}"
        )
      endif ()

      if (NOT EXPORT_EXECUTABLE_LD_FLAGS STREQUAL "")
        set (
          MESSAGE_STATUS
          "${MESSAGE_STATUS}, executable LD flags: ${EXPORT_EXECUTABLE_LD_FLAGS}"
        )
      endif ()

      message (STATUS ${MESSAGE_STATUS})
      break ()
    endif ()
  endforeach ()

  if (NOT TEST_RESULT)
    set (CMAKE_HAVE_EXPORT false)
    set (CMAKE_EXPORT_MODE "")
    set (CMAKE_EXPORT_MODE_UPPERCASE "")
    set (CMAKE_EXPORT_SHARED_LIBRARY_LD_FLAGS "")
    set (CMAKE_EXPORT_EXECUTABLE_LD_FLAGS "")
    message (STATUS "${MESSAGE_PREFIX} - no")
  endif ()

  set (
    CMAKE_HAVE_EXPORT ${CMAKE_HAVE_EXPORT}
    CACHE BOOL "Status of export"
  )
  set (
    CMAKE_EXPORT_MODE ${CMAKE_EXPORT_MODE}
    CACHE STRING "Export mode"
  )
  set (
    CMAKE_EXPORT_MODE_UPPERCASE ${CMAKE_EXPORT_MODE_UPPERCASE}
    CACHE STRING "Export mode uppercase"
  )
  set (
    CMAKE_EXPORT_SHARED_LIBRARY_LD_FLAGS ${CMAKE_EXPORT_SHARED_LIBRARY_LD_FLAGS}
    CACHE STRING "Export shared library LD flags"
  )
  set (
    CMAKE_EXPORT_EXECUTABLE_LD_FLAGS ${CMAKE_EXPORT_EXECUTABLE_LD_FLAGS}
    CACHE STRING "Export executable LD flags"
  )

  mark_as_advanced (
    CMAKE_HAVE_EXPORT
    CMAKE_EXPORT_MODE
    CMAKE_EXPORT_MODE_UPPERCASE
    CMAKE_EXPORT_SHARED_LIBRARY_LD_FLAGS
    CMAKE_EXPORT_EXECUTABLE_LD_FLAGS
  )

  if (NOT CMAKE_HAVE_EXPORT AND CHECK_MODE STREQUAL "REQUIRED")
    message (FATAL_ERROR "${MESSAGE_PREFIX} - is required")
  endif ()
endfunction ()
