// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_STRING_H)
#define LZWS_STRING_H

#include <stdlib.h>

#include "common.h"

// Possible results:
enum {
  LZWS_STRING_ALLOCATE_BUFFER_FAILED = 1,
  LZWS_STRING_COMPRESSOR_FAILED,
  LZWS_STRING_DECOMPRESSOR_FAILED
};

// Destination buffer length can be equal to 0.
// In this case it will be replaced with default value.

lzws_result_t lzws_compress_string(
  uint8_t* source, size_t source_length,
  uint8_t** destination_ptr, size_t* destination_length_ptr, size_t destination_buffer_length,
  uint_fast8_t max_code_bit_length, bool block_mode, bool msb, bool unaligned_bit_groups, bool quiet);

lzws_result_t lzws_decompress_string(
  uint8_t* source, size_t source_length,
  uint8_t** destination_ptr, size_t* destination_length_ptr, size_t destination_buffer_length,
  bool msb, bool unaligned_bit_groups, bool quiet);

#endif // LZWS_STRING_H
