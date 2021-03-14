// LZW streaming decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "wrapper.h"

#include "../utils.h"

LZWS_EXPORT extern inline void lzws_decompressor_initialize_alignment_wrapper(lzws_decompressor_state_t* state_ptr);

LZWS_EXPORT extern inline void lzws_decompressor_add_source_byte_length_to_alignment_wrapper(
  lzws_decompressor_state_t* state_ptr,
  lzws_byte_fast_t           source_byte_length);

LZWS_EXPORT extern inline bool lzws_decompressor_need_to_read_alignment_wrapper(
  const lzws_decompressor_state_t* state_ptr);

static inline lzws_result_t
  read_alignment(lzws_decompressor_state_t* state_ptr, lzws_byte_t** source_ptr, size_t* source_length_ptr)
{
  lzws_decompressor_alignment_t* alignment_ptr = &state_ptr->alignment;
  lzws_byte_fast_t               byte;

  while (lzws_decompressor_need_to_read_alignment_byte(alignment_ptr)) {
    if (*source_length_ptr < 1) {
      return LZWS_DECOMPRESSOR_NEEDS_MORE_SOURCE;
    }

    lzws_decompressor_add_source_byte_length_to_alignment(alignment_ptr, 1);
    lzws_decompressor_read_byte(state_ptr, &byte, source_ptr, source_length_ptr);

    // Some UNIX compress implementations write random bits from uninitialized buffer as alignment bits.
    // There is no guarantee that alignment bits will be zeroes.
    // So in terms of compatibility decompressor have to just ignore alignment bit values.
  }

  lzws_decompressor_reset_alignment_after_reading(alignment_ptr, state_ptr->free_code_bit_length);

  return 0;
}

LZWS_EXPORT lzws_result_t lzws_decompressor_read_alignment_before_first_code(
  lzws_decompressor_state_t* state_ptr,
  lzws_byte_t**              source_ptr,
  size_t*                    source_length_ptr)
{
  lzws_result_t result = read_alignment(state_ptr, source_ptr, source_length_ptr);
  if (result != 0) {
    return result;
  }

  state_ptr->status = LZWS_DECOMPRESSOR_READ_FIRST_CODE;

  return 0;
}

LZWS_EXPORT lzws_result_t lzws_decompressor_read_alignment_before_next_code(
  lzws_decompressor_state_t* state_ptr,
  lzws_byte_t**              source_ptr,
  size_t*                    source_length_ptr)
{
  lzws_result_t result = read_alignment(state_ptr, source_ptr, source_length_ptr);
  if (result != 0) {
    return result;
  }

  state_ptr->status = LZWS_DECOMPRESSOR_READ_NEXT_CODE;

  return 0;
}
