// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "../utils.h"

#include "ratio/wrapper.h"
#include "utils.h"

void lzws_compressor_read_byte(
  lzws_compressor_state_t* state_ptr,
  lzws_byte_fast_t*        byte_ptr,
  lzws_byte_t**            source_ptr,
  size_t*                  source_length_ptr)
{
  lzws_read_byte(byte_ptr, source_ptr, source_length_ptr);

  lzws_compressor_add_source_symbol_to_ratio_wrapper(state_ptr);
}

void lzws_compressor_write_byte(
  lzws_compressor_state_t* state_ptr,
  lzws_byte_fast_t         byte,
  lzws_byte_t**            destination_ptr,
  size_t*                  destination_length_ptr)
{
  if (state_ptr->options.msb) {
    byte = lzws_get_byte_with_reversed_bits(byte);
  }

  lzws_write_byte(byte, destination_ptr, destination_length_ptr);

  lzws_compressor_add_destination_symbol_to_ratio_wrapper(state_ptr);
}
