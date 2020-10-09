// LZW streaming decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "../utils.h"

#include "utils.h"

void lzws_decompressor_read_byte(const lzws_decompressor_state_t* state, lzws_byte_fast_t* byte_ptr,
                                 lzws_byte_t** source_ptr, size_t* source_length_ptr)
{
  lzws_byte_fast_t byte;
  lzws_read_byte(&byte, source_ptr, source_length_ptr);

  if (state->options.msb) {
    byte = lzws_get_byte_with_reversed_bits(byte);
  }

  *byte_ptr = byte;
}
