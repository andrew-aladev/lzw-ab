// LZW streaming decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_DECOMPRESSOR_ALIGNMENT_COMMON_H)
#define LZWS_DECOMPRESSOR_ALIGNMENT_COMMON_H

#include "../../common.h"

typedef struct
{
  lzws_byte_fast_t free_code_bit_length;
  lzws_byte_fast_t source_byte_length;
} lzws_decompressor_alignment_t;

#endif // LZWS_DECOMPRESSOR_ALIGNMENT_COMMON_H
