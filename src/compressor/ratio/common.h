// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_COMPRESSOR_RATIO_COMMON_H)
#define LZWS_COMPRESSOR_RATIO_COMMON_H

#include <stdint.h>

#include "../../bigint/main.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct
{
  uint_fast32_t new_source_length, new_destination_length;
  lzws_bigint_t source_length, destination_length;
} lzws_compressor_ratio_t;

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // LZWS_COMPRESSOR_RATIO_COMMON_H
