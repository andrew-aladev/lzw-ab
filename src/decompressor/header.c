// LZW streaming decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "header.h"

#include "../log.h"
#include "../utils.h"

lzws_result_t lzws_decompressor_read_magic_header(
  lzws_decompressor_state_t* state_ptr,
  lzws_byte_t**              source_ptr,
  size_t*                    source_length_ptr)
{
  if (*source_length_ptr < 2) {
    return LZWS_DECOMPRESSOR_NEEDS_MORE_SOURCE;
  }

  lzws_byte_fast_t byte;
  lzws_read_byte(&byte, source_ptr, source_length_ptr);

  if (byte != LZWS_FIRST_MAGIC_HEADER_BYTE) {
    if (!state_ptr->options.quiet) {
      LZWS_LOG_ERROR("received invalid first magic header byte: %u", byte);
    }

    return LZWS_DECOMPRESSOR_INVALID_MAGIC_HEADER;
  }

  lzws_read_byte(&byte, source_ptr, source_length_ptr);

  if (byte != LZWS_SECOND_MAGIC_HEADER_BYTE) {
    if (!state_ptr->options.quiet) {
      LZWS_LOG_ERROR("received invalid second magic header byte: %u", byte);
    }

    return LZWS_DECOMPRESSOR_INVALID_MAGIC_HEADER;
  }

  state_ptr->status = LZWS_DECOMPRESSOR_READ_HEADER;

  return 0;
}

lzws_result_t lzws_decompressor_read_header(
  lzws_decompressor_state_t* state_ptr,
  lzws_byte_t**              source_ptr,
  size_t*                    source_length_ptr)
{
  // Reading first byte for max code bit length and block mode.
  if (*source_length_ptr < 1) {
    return LZWS_DECOMPRESSOR_NEEDS_MORE_SOURCE;
  }

  lzws_byte_fast_t byte;
  lzws_read_byte(&byte, source_ptr, source_length_ptr);

  lzws_byte_fast_t max_code_bit_length = byte & LZWS_MAX_CODE_BIT_MASK;
  if (max_code_bit_length < LZWS_LOWEST_MAX_CODE_BIT_LENGTH || max_code_bit_length > LZWS_BIGGEST_MAX_CODE_BIT_LENGTH) {
    if (!state_ptr->options.quiet) {
      LZWS_LOG_ERROR("received invalid max code bit length: %u", max_code_bit_length);
    }

    return LZWS_DECOMPRESSOR_INVALID_MAX_CODE_BIT_LENGTH;
  }

  bool block_mode       = (byte & LZWS_BLOCK_MODE) != 0;
  state_ptr->block_mode = block_mode;

  state_ptr->max_code  = lzws_get_max_value_for_bits(max_code_bit_length);
  state_ptr->free_code = state_ptr->first_free_code = lzws_get_first_free_code(block_mode);

  // It is possible to keep prefix code uninitialized.

  state_ptr->status = LZWS_DECOMPRESSOR_ALLOCATE_DICTIONARY;

  return 0;
}
