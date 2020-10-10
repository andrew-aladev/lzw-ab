// LZW streaming decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_DECOMPRESSOR_ALIGNMENT_WRAPPER_H)
#define LZWS_DECOMPRESSOR_ALIGNMENT_WRAPPER_H

#include "../state.h"
#include "main.h"

inline void lzws_decompressor_initialize_alignment_wrapper(lzws_decompressor_state_t* state_ptr)
{
  if (!state_ptr->options.unaligned_bit_groups) {
    lzws_decompressor_initialize_alignment(&state_ptr->alignment, state_ptr->free_code_bit_length);
  }
}

inline void lzws_decompressor_add_source_byte_length_to_alignment_wrapper(
  lzws_decompressor_state_t* state_ptr,
  lzws_byte_fast_t           source_byte_length)
{
  if (!state_ptr->options.unaligned_bit_groups) {
    lzws_decompressor_add_source_byte_length_to_alignment(&state_ptr->alignment, source_byte_length);
  }
}

inline bool lzws_decompressor_need_to_read_alignment_wrapper(const lzws_decompressor_state_t* state_ptr)
{
  if (state_ptr->options.unaligned_bit_groups) {
    return false;
  }

  return lzws_decompressor_need_to_read_alignment(&state_ptr->alignment, state_ptr->free_code_bit_length);
}

lzws_result_t lzws_decompressor_read_alignment_before_first_code(
  lzws_decompressor_state_t* state_ptr,
  lzws_byte_t**              source_ptr,
  size_t*                    source_length_ptr);
lzws_result_t lzws_decompressor_read_alignment_before_next_code(
  lzws_decompressor_state_t* state_ptr,
  lzws_byte_t**              source_ptr,
  size_t*                    source_length_ptr);

#endif // LZWS_DECOMPRESSOR_ALIGNMENT_WRAPPER_H
