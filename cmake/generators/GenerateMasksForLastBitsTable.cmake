set (CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

function (generate_masks_for_last_bits_table)
  if (DEFINED CMAKE_MASKS_FOR_LAST_BITS_TABLE)
    return ()
  endif ()

  set (MESSAGE_PREFIX "Masks for last bits table")

  set (NAME "cmake_generator_masks_for_last_bits_table")
  set (SOURCE_DIR "${CURRENT_LIST_DIR}/masks_for_last_bits_table")
  set (BINARY_DIR "${PROJECT_BINARY_DIR}/generator_masks_for_last_bits_table")

  include (GetVerboseFlags)
  cmake_get_verbose_flags ()

  include (CheckC17)
  cmake_check_c17 ()

  try_compile (
    COMPILE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
    CMAKE_FLAGS
      "-DCMAKE_C_FLAGS=${CMAKE_VERBOSE_C_FLAGS} ${CMAKE_C17_C_FLAGS} ${CMAKE_WERROR_C_FLAGS}"
      "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}"
      "-DCMAKE_GENERATOR_USE_CURRENT_TOOLCHAIN=${CMAKE_GENERATOR_USE_CURRENT_TOOLCHAIN}"
    OUTPUT_VARIABLE COMPILE_OUTPUT
  )

  if (CMAKE_VERBOSE_MAKEFILE)
    message (STATUS ${COMPILE_OUTPUT})
  endif ()

  if (COMPILE_RESULT)
    execute_process (
      COMMAND "${BINARY_DIR}/main"
      RESULT_VARIABLE RUN_RESULT
      OUTPUT_VARIABLE RUN_OUTPUT
      ERROR_VARIABLE RUN_ERRORS
    )

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

  file (REMOVE_RECURSE ${BINARY_DIR})
endfunction ()
