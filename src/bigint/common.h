// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_BIGINT_COMMON_H)
#define LZWS_BIGINT_COMMON_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Possible results:
enum
{
  LZWS_BIGINT_ALLOCATE_FAILED = 1,
  LZWS_BIGINT_UNKNOWN_ERROR
};

// Possible compare results:
enum
{
  LZWS_BIGINT_COMPARE_EQUALS = 0,
  LZWS_BIGINT_COMPARE_LESS_THAN,
  LZWS_BIGINT_COMPARE_GREATER_THAN
};

typedef uint_fast8_t lzws_bigint_compare_result_t;

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LZWS_BIGINT_COMMON_H
