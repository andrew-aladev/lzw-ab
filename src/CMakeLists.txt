if (LZWS_COMPRESSOR_DICTIONARY STREQUAL "linked-list")
  message (STATUS "Dictionary - linked list")
  set (LZWS_COMPRESSOR_DICTIONARY_LINKED_LIST true)
elseif (LZWS_COMPRESSOR_DICTIONARY STREQUAL "sparse-array")
  message (STATUS "Dictionary - sparse array")
  set (LZWS_COMPRESSOR_DICTIONARY_SPARSE_ARRAY true)
else ()
  message (FATAL_ERROR "Invalid compressor dictionary implementation: ${LZWS_COMPRESSOR_DICTIONARY}")
endif ()

if (LZWS_BIGNUM_LIBRARY STREQUAL "gmp")
  message (STATUS "Bignum library - GMP")
  set (LZWS_BIGNUM_LIBRARY_GMP true)
elseif (LZWS_BIGNUM_LIBRARY STREQUAL "tommath")
  message (STATUS "Bignum library - TomMath")
  set (LZWS_BIGNUM_LIBRARY_TOMMATH true)
else ()
  message (FATAL_ERROR "Invalid bignum library: ${LZWS_BIGNUM_LIBRARY}")
endif ()

configure_file ("config.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/config.h")

include (GenerateBytesWithReversedBitsTable)
cmake_generate_bytes_with_reversed_bits_table ()

include (GenerateMasksForLastBitsTable)
cmake_generate_masks_for_last_bits_table ()

if (DEFINED CMAKE_BYTES_WITH_REVERSED_BITS_TABLE AND DEFINED CMAKE_MASKS_FOR_LAST_BITS_TABLE)
  configure_file ("generated_constants.c.in" "${CMAKE_CURRENT_SOURCE_DIR}/generated_constants.c")
endif ()

add_subdirectory ("bigint")
add_subdirectory ("compressor")
add_subdirectory ("decompressor")

set (HEADERS ${HEADERS}
  "${CMAKE_CURRENT_SOURCE_DIR}/buffer.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/common.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/config.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/constants.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/file.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/log.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/macro.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/string.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/utils.h"
)
set (SOURCES ${SOURCES}
  "${CMAKE_CURRENT_SOURCE_DIR}/buffer.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/common.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/file.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/generated_constants.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/string.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/utils.c"
)

include (DefineRelativeFilePaths)
cmake_define_relative_file_paths ("${SOURCES}")

include_directories (${CMAKE_BIGNUM_LIBRARY_INCLUDE_PATH})

foreach (HEADER ${HEADERS})
  file (RELATIVE_PATH HEADER_RELATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${HEADER})
  get_filename_component (HEADER_RELATIVE_DIRECTORY_PATH ${HEADER_RELATIVE_PATH} DIRECTORY)
  install (FILES ${HEADER} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${LZWS_TARGET}/${HEADER_RELATIVE_DIRECTORY_PATH}")
endforeach ()

if (LZWS_SHARED)
  set (LIB_NAME ${LZWS_TARGET})
  string (TOUPPER ${LZWS_TARGET} LIB_TARGET)

  if (NOT DEFINED ANY_LIB_TARGET)
    set (ANY_LIB_TARGET ${LIB_TARGET})
  endif ()

  add_library (${LIB_TARGET} SHARED ${SOURCES})
  target_link_libraries (${LIB_TARGET} ${CMAKE_BIGNUM_LIBRARY_SHARED_PATH})
  set_target_properties (${LIB_TARGET} PROPERTIES OUTPUT_NAME ${LIB_NAME})

  install (TARGETS ${LIB_TARGET} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif ()

if (LZWS_STATIC)
  set (STATIC_LIB_NAME "${LZWS_TARGET}-static")
  string (TOUPPER "${LZWS_TARGET}_static" STATIC_LIB_TARGET)

  if (NOT DEFINED ANY_LIB_TARGET)
    set (ANY_LIB_TARGET ${STATIC_LIB_TARGET})
  endif ()

  add_library (${STATIC_LIB_TARGET} STATIC ${SOURCES})
  target_link_libraries (${STATIC_LIB_TARGET} ${CMAKE_BIGNUM_LIBRARY_STATIC_PATH})
  set_target_properties (${STATIC_LIB_TARGET} PROPERTIES OUTPUT_NAME ${STATIC_LIB_NAME})

  install (TARGETS ${STATIC_LIB_TARGET} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif ()

# Export.
string (TOUPPER ${LZWS_TARGET} LIB_BASE_NAME)

include (GenerateExportHeader)
generate_export_header (
  ${ANY_LIB_TARGET}
  BASE_NAME ${LIB_BASE_NAME}
  EXPORT_FILE_NAME "${CMAKE_CURRENT_SOURCE_DIR}/export.h"
)

if (LZWS_STATIC)
  set_target_properties(${STATIC_LIB_TARGET} PROPERTIES COMPILE_FLAGS "-D${LIB_BASE_NAME}_STATIC_DEFINE")
endif ()

install (FILES "${CMAKE_CURRENT_SOURCE_DIR}/export.h" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${LZWS_TARGET}")

# Other.
if (LZWS_CLI)
  add_subdirectory ("cli")
endif ()

if (LZWS_EXAMPLES)
  add_subdirectory ("examples")
endif ()

if (LZWS_TESTS)
  add_subdirectory ("tests")
endif ()
