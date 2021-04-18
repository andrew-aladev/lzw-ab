set (
  HEADERS ${HEADERS}
  "${CMAKE_CURRENT_SOURCE_DIR}/common.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/main.h"
)

if (LZWS_BIGNUM_LIBRARY STREQUAL "gmp")
  set (
    HEADERS ${HEADERS}
    "${CMAKE_CURRENT_SOURCE_DIR}/gmp_int.h"
  )
  set (
    SOURCES ${SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/gmp_int.c"
  )
elseif (LZWS_BIGNUM_LIBRARY STREQUAL "tommath")
  set (
    HEADERS ${HEADERS}
    "${CMAKE_CURRENT_SOURCE_DIR}/tommath_int.h"
  )
  set (
    SOURCES ${SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/tommath_int.c"
  )
else ()
  message (FATAL_ERROR "Invalid bignum library: ${LZWS_BIGNUM_LIBRARY}")
endif ()

set (HEADERS ${HEADERS} PARENT_SCOPE)
set (SOURCES ${SOURCES} PARENT_SCOPE)
