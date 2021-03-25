// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_COMPRESSOR_ALIGNMENT_COMMON_H)
#define LZWS_COMPRESSOR_ALIGNMENT_COMMON_H

#include "../../common.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct
{
  lzws_byte_fast_t last_used_code_bit_length;
  lzws_byte_fast_t destination_byte_length;
} lzws_compressor_alignment_t;

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // LZWS_COMPRESSOR_ALIGNMENT_COMMON_H
