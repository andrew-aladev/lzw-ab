if (LZWS_COMPRESSOR_DICTIONARY_LINKED_LIST)
  add_subdirectory ("linked-list")
elseif (LZWS_COMPRESSOR_DICTIONARY_SPARSE_ARRAY)
  add_subdirectory ("sparse-array")
endif ()

set (
  HEADERS ${HEADERS}
  "${CMAKE_CURRENT_SOURCE_DIR}/common.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/wrapper.h"
  PARENT_SCOPE
)

set (SOURCES ${SOURCES} PARENT_SCOPE)
