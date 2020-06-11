// LZW streaming decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "current_code.h"

#include "../log.h"
#include "../utils.h"
#include "code.h"
#include "common.h"
#include "dictionary/wrapper.h"
#include "remainder.h"

lzws_result_t lzws_decompressor_read_first_code(lzws_decompressor_state_t* state_ptr, lzws_byte_t** source_ptr, size_t* source_length_ptr)
{
  lzws_code_fast_t code;

  lzws_result_t result = lzws_decompressor_read_code(state_ptr, &code, source_ptr, source_length_ptr);
  if (result != 0) {
    return result;
  }

  if (state_ptr->block_mode && code == LZWS_CLEAR_CODE) {
    // Some UNIX compress implementations can provide clear code as first code.
    // So in terms of compatibility decompressor have to just ignore it.

    // We are reading first code, so we don't need to clear state and remainder, it has been already done.

    if (!state_ptr->unaligned_bit_groups) {
      // We need to read alignment after reading clear code.
      state_ptr->status = LZWS_DECOMPRESSOR_READ_ALIGNMENT_BEFORE_FIRST_CODE;
    }

    // It is possible to keep prefix code in state as is.

    return 0;
  }

  if (code >= LZWS_ALPHABET_LENGTH) {
    if (!state_ptr->quiet) {
      LZWS_LOG_ERROR("received first code: " FAST_CODE_FORMAT " greater than max first code: %u", code, LZWS_ALPHABET_LENGTH - 1);
    }

    return LZWS_DECOMPRESSOR_CORRUPTED_SOURCE;
  }

  state_ptr->prefix_code = code;
  state_ptr->status      = LZWS_DECOMPRESSOR_WRITE_FIRST_SYMBOL;

  return 0;
}

static inline lzws_code_fast_t get_next_code(lzws_decompressor_state_t* state_ptr)
{
  lzws_code_fast_t next_code = state_ptr->free_code;

  if (next_code == state_ptr->max_code) {
    state_ptr->free_code = LZWS_UNDEFINED_FREE_CODE;
  }
  else {
    if (next_code == state_ptr->max_free_code_for_bit_length) {
      lzws_byte_fast_t free_code_bit_length   = ++state_ptr->free_code_bit_length;
      state_ptr->max_free_code_for_bit_length = lzws_get_max_value_for_bits(free_code_bit_length);
    }

    state_ptr->free_code++;
  }

  return next_code;
}

lzws_result_t lzws_decompressor_read_next_code(lzws_decompressor_state_t* state_ptr, lzws_byte_t** source_ptr, size_t* source_length_ptr)
{
  lzws_code_fast_t code;

  lzws_result_t result = lzws_decompressor_read_code(state_ptr, &code, source_ptr, source_length_ptr);
  if (result != 0) {
    return result;
  }

  if (state_ptr->block_mode && code == LZWS_CLEAR_CODE) {
    // Some UNIX compress implementations provide clear code even when dictionary is not full.
    // So in terms of compatibility decompressor have to just ignore situation when dictionary is not full here.

    // We need to clear state after reading clear code.
    lzws_decompressor_clear_state(state_ptr);

    // It is possible to keep prefix code in state as is.
    // Algorithm won't touch it without reinitialization.

    if (state_ptr->unaligned_bit_groups) {
      state_ptr->status = LZWS_DECOMPRESSOR_READ_FIRST_CODE;
    }
    else {
      // Remainder is a part of alignment, we need to clear it.
      lzws_decompressor_clear_remainder(state_ptr);

      // We need to read alignment after reading clear code.
      state_ptr->status = LZWS_DECOMPRESSOR_READ_ALIGNMENT_BEFORE_FIRST_CODE;
    }

    return 0;
  }

  if (lzws_decompressor_is_dictionary_full(state_ptr)) {
    lzws_decompressor_write_code_to_dictionary_wrapper(state_ptr, code);
  }
  else {
    lzws_code_fast_t next_code = get_next_code(state_ptr);
    if (code > next_code) {
      if (!state_ptr->quiet) {
        LZWS_LOG_ERROR("received code: " FAST_CODE_FORMAT " greater than next code: " FAST_CODE_FORMAT, code, next_code);
      }

      return LZWS_DECOMPRESSOR_CORRUPTED_SOURCE;
    }

    lzws_decompressor_add_code_to_dictionary_wrapper(state_ptr, state_ptr->prefix_code, code, next_code);
  }

  state_ptr->prefix_code = code;
  state_ptr->status      = LZWS_DECOMPRESSOR_WRITE_SYMBOLS_FOR_CURRENT_CODE;

  return 0;
}
