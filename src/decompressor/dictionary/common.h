// LZW streaming decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_DECOMPRESSOR_DICTIONARY_COMMON_H)
#define LZWS_DECOMPRESSOR_DICTIONARY_COMMON_H

#include <stdlib.h>

#include "../common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  lzws_code_t* previous_codes;
  lzws_byte_t* last_symbol_by_codes;

  lzws_code_fast_t codes_offset;

  lzws_byte_t* output_buffer;
  size_t       output_length;
} lzws_decompressor_dictionary_t;

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LZWS_DECOMPRESSOR_DICTIONARY_COMMON_H
