// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "header.h"

#include "../utils.h"

lzws_result_t lzws_compressor_write_magic_header(
  lzws_compressor_state_t* state_ptr,
  lzws_byte_t**            destination_ptr,
  size_t*                  destination_length_ptr)
{
  if (*destination_length_ptr < 2) {
    return LZWS_COMPRESSOR_NEEDS_MORE_DESTINATION;
  }

  lzws_write_byte(LZWS_FIRST_MAGIC_HEADER_BYTE, destination_ptr, destination_length_ptr);
  lzws_write_byte(LZWS_SECOND_MAGIC_HEADER_BYTE, destination_ptr, destination_length_ptr);

  state_ptr->status = LZWS_COMPRESSOR_WRITE_HEADER;

  return 0;
}

lzws_result_t lzws_compressor_write_header(
  lzws_compressor_state_t* state_ptr,
  lzws_byte_t**            destination_ptr,
  size_t*                  destination_length_ptr)
{
  // Writing first byte for max code bit length and block mode.
  if (*destination_length_ptr < 1) {
    return LZWS_COMPRESSOR_NEEDS_MORE_DESTINATION;
  }

  lzws_byte_fast_t byte = state_ptr->options.max_code_bit_length;
  if (state_ptr->options.block_mode) {
    byte = byte | LZWS_BLOCK_MODE;
  }

  lzws_write_byte(byte, destination_ptr, destination_length_ptr);

  state_ptr->status = LZWS_COMPRESSOR_ALLOCATE_DICTIONARY;

  return 0;
}
