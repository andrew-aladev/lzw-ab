// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "code.h"

#include "../utils.h"
#include "alignment/wrapper.h"
#include "utils.h"

static inline lzws_byte_fast_t get_byte(lzws_code_fast_t* code_ptr, lzws_byte_fast_t* code_bit_length_ptr, bool msb)
{
  lzws_code_fast_t code                      = *code_ptr;
  lzws_byte_fast_t code_bit_length           = *code_bit_length_ptr;
  lzws_byte_fast_t remaining_code_bit_length = code_bit_length - 8;
  lzws_byte_fast_t byte;

  if (msb) {
    // Taking first bits from code.
    byte = code >> remaining_code_bit_length;

    // Removing first bits from code.
    code &= lzws_get_mask_for_last_bits(remaining_code_bit_length);
  } else {
    // Taking last bits from code.
    byte = code & 0xff;

    // Removing last bits from code.
    code >>= 8;
  }

  *code_ptr            = code;
  *code_bit_length_ptr = remaining_code_bit_length;

  return byte;
}

static inline lzws_byte_fast_t get_byte_with_remainder(
  lzws_code_fast_t* code_ptr,
  lzws_byte_fast_t* code_bit_length_ptr,
  lzws_byte_fast_t  remainder,
  lzws_byte_fast_t  remainder_bit_length,
  bool              msb)
{
  if (remainder_bit_length == 0) {
    return get_byte(code_ptr, code_bit_length_ptr, msb);
  }

  lzws_code_fast_t code                      = *code_ptr;
  lzws_byte_fast_t code_bit_length           = *code_bit_length_ptr;
  lzws_byte_fast_t current_code_bit_length   = 8 - remainder_bit_length;
  lzws_byte_fast_t remaining_code_bit_length = code_bit_length - current_code_bit_length;
  lzws_byte_fast_t byte;

  if (msb) {
    // Taking first bits from code.
    byte = code >> remaining_code_bit_length;

    // Remainder is sitting on the top.
    byte = (remainder << current_code_bit_length) | byte;

    // Removing first bits from code.
    code &= lzws_get_mask_for_last_bits(remaining_code_bit_length);
  } else {
    // Taking last bits from code.
    byte = code & lzws_get_mask_for_last_bits(current_code_bit_length);

    // Remainder is sitting on the bottom.
    byte = (byte << remainder_bit_length) | remainder;

    // Removing last bits from code.
    code >>= current_code_bit_length;
  }

  *code_ptr            = code;
  *code_bit_length_ptr = remaining_code_bit_length;

  return byte;
}

lzws_result_t lzws_compressor_write_code(
  lzws_compressor_state_t* state_ptr,
  lzws_code_fast_t         code,
  lzws_byte_t**            destination_ptr,
  size_t*                  destination_length_ptr)
{
  lzws_byte_fast_t code_bit_length      = state_ptr->last_used_code_bit_length;
  lzws_byte_fast_t remainder_bit_length = state_ptr->remainder_bit_length;

  // Code bit length will always be >= 8.
  // Remainder bit length will always be <= 7.
  // So destination byte length will always be >= 1.
  lzws_byte_fast_t destination_byte_length =
    lzws_floor_bit_length_to_byte_length(code_bit_length + remainder_bit_length);

  if (*destination_length_ptr < destination_byte_length) {
    return LZWS_COMPRESSOR_NEEDS_MORE_DESTINATION;
  }

  lzws_compressor_add_destination_byte_length_to_alignment_wrapper(state_ptr, destination_byte_length);

  lzws_byte_fast_t remainder = state_ptr->remainder;
  lzws_byte_fast_t byte =
    get_byte_with_remainder(&code, &code_bit_length, remainder, remainder_bit_length, state_ptr->options.msb);

  lzws_compressor_write_byte(state_ptr, byte, destination_ptr, destination_length_ptr);

  destination_byte_length--;

  while (destination_byte_length != 0) {
    byte = get_byte(&code, &code_bit_length, state_ptr->options.msb);
    lzws_compressor_write_byte(state_ptr, byte, destination_ptr, destination_length_ptr);

    destination_byte_length--;
  }

  state_ptr->remainder            = code;
  state_ptr->remainder_bit_length = code_bit_length;

  return 0;
}
