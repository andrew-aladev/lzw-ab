set (HEADERS ${HEADERS}
  "${CMAKE_CURRENT_SOURCE_DIR}/common.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/main.h"
  PARENT_SCOPE
)

if (LZWS_BIGNUM_LIBRARY STREQUAL "gmp")
  set (HEADERS ${HEADERS}
    "${CMAKE_CURRENT_SOURCE_DIR}/gmp_int.h"
    PARENT_SCOPE
  )
  set (SOURCES ${SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/gmp_int.c"
    PARENT_SCOPE
  )
elseif (LZWS_BIGNUM_LIBRARY STREQUAL "tommath")
  set (HEADERS ${HEADERS}
    "${CMAKE_CURRENT_SOURCE_DIR}/tommath_int.h"
    PARENT_SCOPE
  )
  set (SOURCES ${SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/tommath_int.c"
    PARENT_SCOPE
  )
else ()
  message (FATAL_ERROR "Invalid bignum library: ${LZWS_BIGNUM_LIBRARY}")
endif ()
