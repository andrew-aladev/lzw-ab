set (CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

function (cmake_run_masks_for_last_bits_table)
  set (NAME "cmake_run_masks_for_last_bits_table")
  set (SOURCE_DIR "${CURRENT_LIST_DIR}/masks_for_last_bits_table")
  set (BINARY_DIR "${PROJECT_BINARY_DIR}/run_masks_for_last_bits_table")

  include (GetVerboseFlags)
  cmake_get_verbose_flags ()

  include (GetBuildFlags)
  cmake_get_build_flags ()

  include (CheckC11AndAbove)
  cmake_check_c11_and_above (REQUIRED)

  try_compile (
    COMPILE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
    CMAKE_FLAGS
      "-DCMAKE_C_FLAGS=${CMAKE_VERBOSE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE} ${CMAKE_C11_AND_ABOVE_C_FLAGS}"
      "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}"
      "-DCMAKE_GENERATOR_USE_CURRENT_TOOLCHAIN=${CMAKE_GENERATOR_USE_CURRENT_TOOLCHAIN}"
    OUTPUT_VARIABLE COMPILE_OUTPUT
  )

  if (CMAKE_VERBOSE_MAKEFILE)
    message (STATUS ${COMPILE_OUTPUT})
  endif ()

  if (COMPILE_RESULT)
    execute_process (
      COMMAND "${BINARY_DIR}/result"
      RESULT_VARIABLE RUN_RESULT
      OUTPUT_VARIABLE RUN_OUTPUT
      ERROR_VARIABLE RUN_ERRORS
    )
  endif ()

  file (REMOVE_RECURSE ${BINARY_DIR})

  set (COMPILE_RESULT ${COMPILE_RESULT} PARENT_SCOPE)
  set (RUN_RESULT ${RUN_RESULT} PARENT_SCOPE)
  set (RUN_OUTPUT ${RUN_OUTPUT} PARENT_SCOPE)
  set (RUN_ERRORS ${RUN_ERRORS} PARENT_SCOPE)
endfunction ()

function (cmake_generate_masks_for_last_bits_table)
  if (DEFINED CMAKE_MASKS_FOR_LAST_BITS_TABLE)
    return ()
  endif ()

  cmake_run_masks_for_last_bits_table ()

  set (MESSAGE_PREFIX "Masks for last bits table")

  if (COMPILE_RESULT)
    if (RUN_RESULT EQUAL 0)
      set (CMAKE_MASKS_FOR_LAST_BITS_TABLE ${RUN_OUTPUT} PARENT_SCOPE)
      message (STATUS "${MESSAGE_PREFIX} - generated")
    else ()
      unset (CMAKE_MASKS_FOR_LAST_BITS_TABLE PARENT_SCOPE)
      message (STATUS "${MESSAGE_PREFIX} - result: ${RUN_RESULT}, errors:\n${RUN_ERRORS}")
      message (STATUS "${MESSAGE_PREFIX} - failed to generate, using default")
    endif ()

  else ()
    unset (CMAKE_MASKS_FOR_LAST_BITS_TABLE PARENT_SCOPE)
    message (STATUS "${MESSAGE_PREFIX} - compilation failed, using default")
  endif ()
endfunction ()
