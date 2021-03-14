// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "current_code.h"

#include "../utils.h"
#include "code.h"
#include "dictionary/wrapper.h"
#include "ratio/wrapper.h"

static inline lzws_result_t
  write_current_code(lzws_compressor_state_t* state_ptr, lzws_byte_t** destination_ptr, size_t* destination_length_ptr)
{
  return lzws_compressor_write_code(state_ptr, state_ptr->current_code, destination_ptr, destination_length_ptr);
}

static inline lzws_code_fast_t get_next_code(lzws_compressor_state_t* state_ptr)
{
  if (state_ptr->last_used_code == state_ptr->max_last_used_code_for_bit_length) {
    lzws_byte_fast_t last_used_code_bit_length   = ++state_ptr->last_used_code_bit_length;
    state_ptr->max_last_used_code_for_bit_length = lzws_get_max_value_for_bits(last_used_code_bit_length);
  }

  return ++state_ptr->last_used_code;
}

LZWS_EXPORT lzws_result_t lzws_compressor_write_current_code(
  lzws_compressor_state_t* state_ptr,
  lzws_byte_t**            destination_ptr,
  size_t*                  destination_length_ptr)
{
  lzws_result_t result = write_current_code(state_ptr, destination_ptr, destination_length_ptr);
  if (result != 0) {
    return result;
  }

  lzws_code_fast_t current_code = state_ptr->current_code;
  lzws_byte_fast_t next_symbol  = state_ptr->next_symbol;

  // We need to set current code to next symbol in all possible cases.
  state_ptr->current_code = next_symbol;

  // It is possible to keep next symbol in state as is.
  // Algorithm won't touch it without reinitialization.

  if (state_ptr->options.block_mode && current_code == LZWS_CLEAR_CODE) {
    // We need to clear state after writing clear code.
    result = lzws_compressor_clear_state(state_ptr);
    if (result != 0) {
      return result;
    }

    if (state_ptr->options.unaligned_bit_groups) {
      state_ptr->status = LZWS_COMPRESSOR_READ_NEXT_SYMBOL;
    } else {
      // We need to write alignment after writing clear code (there will be at least one code after it).
      state_ptr->status = LZWS_COMPRESSOR_WRITE_REMAINDER_BEFORE_READ_NEXT_SYMBOL;
    }

    return 0;
  }

  if (!lzws_compressor_is_dictionary_full(state_ptr)) {
    lzws_code_fast_t next_code = get_next_code(state_ptr);
    lzws_compressor_save_next_code_to_dictionary_wrapper(state_ptr, current_code, next_symbol, next_code);

    if (lzws_compressor_is_dictionary_full(state_ptr)) {
      // Dictionary become full.
      // We need to clear ratio now.
      result = lzws_compressor_clear_ratio_wrapper(state_ptr);
      if (result != 0) {
        return result;
      }
    }
  }

  state_ptr->status = LZWS_COMPRESSOR_READ_NEXT_SYMBOL;

  return 0;
}

LZWS_EXPORT lzws_result_t lzws_compressor_flush_current_code(
  lzws_compressor_state_t* state_ptr,
  lzws_byte_t**            destination_ptr,
  size_t*                  destination_length_ptr)
{
  lzws_result_t result = write_current_code(state_ptr, destination_ptr, destination_length_ptr);
  if (result != 0) {
    return result;
  }

  state_ptr->status = LZWS_COMPRESSOR_FLUSH_REMAINDER;

  return 0;
}
