// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_COMPRESSOR_ALIGNMENT_WRAPPER_H)
#define LZWS_COMPRESSOR_ALIGNMENT_WRAPPER_H

#include "../state.h"
#include "main.h"

#if defined(__cplusplus)
extern "C" {
#endif

LZWS_EXPORT inline void lzws_compressor_initialize_alignment_wrapper(lzws_compressor_state_t* state_ptr)
{
  if (!state_ptr->options.unaligned_bit_groups) {
    lzws_compressor_initialize_alignment(&state_ptr->alignment, state_ptr->last_used_code_bit_length);
  }
}

LZWS_EXPORT inline void lzws_compressor_add_destination_byte_length_to_alignment_wrapper(
  lzws_compressor_state_t* state_ptr,
  lzws_byte_fast_t         destination_byte_length)
{
  if (!state_ptr->options.unaligned_bit_groups) {
    lzws_compressor_add_destination_byte_length_to_alignment(&state_ptr->alignment, destination_byte_length);
  }
}

LZWS_EXPORT inline bool lzws_compressor_need_to_write_alignment_wrapper(const lzws_compressor_state_t* state_ptr)
{
  if (state_ptr->options.unaligned_bit_groups) {
    return false;
  }

  return lzws_compressor_need_to_write_alignment(&state_ptr->alignment, state_ptr->last_used_code_bit_length);
}

LZWS_EXPORT lzws_result_t lzws_compressor_write_alignment_before_read_next_symbol(
  lzws_compressor_state_t* state_ptr,
  lzws_byte_t**            destination_ptr,
  size_t*                  destination_length_ptr);

LZWS_EXPORT lzws_result_t lzws_compressor_write_alignment_before_current_code(
  lzws_compressor_state_t* state_ptr,
  lzws_byte_t**            destination_ptr,
  size_t*                  destination_length_ptr);

LZWS_EXPORT lzws_result_t lzws_compressor_flush_alignment_before_current_code(
  lzws_compressor_state_t* state_ptr,
  lzws_byte_t**            destination_ptr,
  size_t*                  destination_length_ptr);

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // LZWS_COMPRESSOR_ALIGNMENT_WRAPPER_H
