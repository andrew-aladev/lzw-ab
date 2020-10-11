// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "wrapper.h"

#include "../utils.h"

extern inline void lzws_compressor_initialize_alignment_wrapper(lzws_compressor_state_t* state_ptr);

extern inline void lzws_compressor_add_destination_byte_length_to_alignment_wrapper(
  lzws_compressor_state_t* state_ptr,
  lzws_byte_fast_t         destination_byte_length);

extern inline bool lzws_compressor_need_to_write_alignment_wrapper(const lzws_compressor_state_t* state_ptr);

static inline lzws_result_t
  write_alignment(lzws_compressor_state_t* state_ptr, lzws_byte_t** destination_ptr, size_t* destination_length_ptr)
{
  lzws_compressor_alignment_t* alignment_ptr = &state_ptr->alignment;
  lzws_byte_fast_t             byte          = 0;

  while (lzws_compressor_need_to_write_alignment_byte(alignment_ptr)) {
    if (*destination_length_ptr < 1) {
      return LZWS_COMPRESSOR_NEEDS_MORE_DESTINATION;
    }

    lzws_compressor_add_destination_byte_length_to_alignment(alignment_ptr, 1);
    lzws_compressor_write_byte(state_ptr, byte, destination_ptr, destination_length_ptr);
  }

  lzws_compressor_reset_alignment_after_writing(alignment_ptr, state_ptr->last_used_code_bit_length);

  return 0;
}

lzws_result_t lzws_compressor_write_alignment_before_read_next_symbol(
  lzws_compressor_state_t* state_ptr,
  lzws_byte_t**            destination_ptr,
  size_t*                  destination_length_ptr)
{
  lzws_result_t result = write_alignment(state_ptr, destination_ptr, destination_length_ptr);
  if (result != 0) {
    return result;
  }

  state_ptr->status = LZWS_COMPRESSOR_READ_NEXT_SYMBOL;

  return 0;
}

lzws_result_t lzws_compressor_write_alignment_before_current_code(
  lzws_compressor_state_t* state_ptr,
  lzws_byte_t**            destination_ptr,
  size_t*                  destination_length_ptr)
{
  lzws_result_t result = write_alignment(state_ptr, destination_ptr, destination_length_ptr);
  if (result != 0) {
    return result;
  }

  state_ptr->status = LZWS_COMPRESSOR_WRITE_CURRENT_CODE;

  return 0;
}

lzws_result_t lzws_compressor_flush_alignment_before_current_code(
  lzws_compressor_state_t* state_ptr,
  lzws_byte_t**            destination_ptr,
  size_t*                  destination_length_ptr)
{
  lzws_result_t result = write_alignment(state_ptr, destination_ptr, destination_length_ptr);
  if (result != 0) {
    return result;
  }

  state_ptr->status = LZWS_COMPRESSOR_FLUSH_CURRENT_CODE;

  return 0;
}
