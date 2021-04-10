// LZW streaming decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "code.h"

#include "../utils.h"
#include "alignment/wrapper.h"
#include "utils.h"

static inline void
  add_byte(lzws_code_fast_t* code_ptr, lzws_byte_fast_t code_bit_length, lzws_byte_fast_t byte, bool msb)
{
  lzws_code_fast_t code = *code_ptr;

  if (msb) {
    // Code is sitting on the top.
    code = (code << 8) | byte;
  } else {
    // Code is sitting on the bottom.
    code = (lzws_code_fast_t)(byte << code_bit_length) | code;
  }

  *code_ptr = code;
}

static inline void add_byte_with_remainder(
  lzws_code_fast_t* code_ptr,
  lzws_byte_fast_t  code_bit_length,
  lzws_byte_fast_t  target_code_bit_length,
  lzws_byte_fast_t  byte,
  lzws_byte_fast_t* remainder_ptr,
  lzws_byte_fast_t* remainder_bit_length_ptr,
  bool              msb)
{
  lzws_byte_fast_t code_part_bit_length = (lzws_byte_fast_t)(target_code_bit_length - code_bit_length);
  if (code_part_bit_length == 8) {
    add_byte(code_ptr, code_bit_length, byte, msb);

    *remainder_ptr            = 0;
    *remainder_bit_length_ptr = 0;

    return;
  }

  lzws_code_fast_t code = *code_ptr;
  lzws_byte_fast_t code_part;
  lzws_byte_fast_t remainder;
  lzws_byte_fast_t remainder_bit_length = (lzws_byte_fast_t)(8 - code_part_bit_length);

  if (msb) {
    // Taking first bits from byte.
    code_part = (lzws_byte_fast_t)(byte >> remainder_bit_length);

    // Last bits of byte is a remainder.
    remainder = (lzws_byte_fast_t)(byte & lzws_get_mask_for_last_bits(remainder_bit_length));

    // Code is sitting on the top.
    code = (lzws_code_fast_t)(code << code_part_bit_length) | code_part;
  } else {
    // Taking last bits from byte.
    code_part = (lzws_byte_fast_t)(byte & lzws_get_mask_for_last_bits(code_part_bit_length));

    // First bits of byte is a remainder.
    remainder = (lzws_byte_fast_t)(byte >> code_part_bit_length);

    // Code is sitting on the bottom.
    code = (lzws_code_fast_t)(code_part << code_bit_length) | code;
  }

  *code_ptr                 = code;
  *remainder_ptr            = remainder;
  *remainder_bit_length_ptr = remainder_bit_length;
}

LZWS_EXPORT lzws_result_t lzws_decompressor_read_code(
  lzws_decompressor_state_t* state_ptr,
  lzws_code_fast_t*          code_ptr,
  lzws_byte_t**              source_ptr,
  size_t*                    source_length_ptr)
{
  lzws_byte_fast_t target_code_bit_length = state_ptr->free_code_bit_length;
  lzws_byte_fast_t remainder_bit_length   = state_ptr->remainder_bit_length;

  // Target code bit length will always be >= 8.
  // Remainder bit length will always be <= 7.
  // So source byte length will always be >= 1.
  lzws_byte_fast_t source_byte_length =
    (lzws_byte_fast_t) lzws_ceil_bit_length_to_byte_length((size_t)(target_code_bit_length - remainder_bit_length));

  if (*source_length_ptr < source_byte_length) {
    return LZWS_DECOMPRESSOR_NEEDS_MORE_SOURCE;
  }

  lzws_decompressor_add_source_byte_length_to_alignment_wrapper(state_ptr, source_byte_length);

  lzws_code_fast_t code            = state_ptr->remainder;
  lzws_byte_fast_t code_bit_length = remainder_bit_length;
  lzws_byte_fast_t byte;

  while (source_byte_length != 1) {
    lzws_decompressor_read_byte(state_ptr, &byte, source_ptr, source_length_ptr);
    add_byte(&code, code_bit_length, byte, state_ptr->options.msb);

    code_bit_length = (lzws_byte_fast_t)(code_bit_length + 8);
    source_byte_length--;
  }

  lzws_decompressor_read_byte(state_ptr, &byte, source_ptr, source_length_ptr);

  add_byte_with_remainder(
    &code,
    code_bit_length,
    target_code_bit_length,
    byte,
    &state_ptr->remainder,
    &state_ptr->remainder_bit_length,
    state_ptr->options.msb);

  *code_ptr = code;

  return 0;
}
