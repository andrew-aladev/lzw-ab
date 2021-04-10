// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "symbol.h"

#include "alignment/wrapper.h"
#include "dictionary/wrapper.h"
#include "ratio/wrapper.h"
#include "utils.h"

LZWS_EXPORT lzws_result_t lzws_compressor_read_first_symbol(
  lzws_compressor_state_t* state_ptr,
  lzws_byte_t**            source_ptr,
  size_t*                  source_length_ptr)
{
  if (*source_length_ptr < 1) {
    return LZWS_COMPRESSOR_NEEDS_MORE_SOURCE;
  }

  lzws_byte_fast_t symbol;
  lzws_compressor_read_byte(state_ptr, &symbol, source_ptr, source_length_ptr);

  state_ptr->current_code = symbol;
  state_ptr->status       = LZWS_COMPRESSOR_READ_NEXT_SYMBOL;

  return 0;
}

LZWS_EXPORT lzws_result_t lzws_compressor_read_next_symbol(
  lzws_compressor_state_t* state_ptr,
  lzws_byte_t**            source_ptr,
  size_t*                  source_length_ptr)
{
  if (*source_length_ptr < 1) {
    return LZWS_COMPRESSOR_NEEDS_MORE_SOURCE;
  }

  // We want to clear dictionary when there is at least one symbol that will use new dictionary.
  // So we need to check whether we need to clear dictionary before reading this symbol.

  // Current code >= first free code means that dictionary is still working on current source sequence.
  // We can't clear dictionary during source sequence.
  // We can ignore situation when current code equals clear code.
  // So we can compare current code with alphabet length.

  bool          need_to_clear_by_ratio_wrapper;
  lzws_result_t result = lzws_compressor_need_to_clear_by_ratio_wrapper(state_ptr, &need_to_clear_by_ratio_wrapper);
  if (result != 0) {
    return result;
  }

  lzws_code_fast_t current_code = state_ptr->current_code;
  if (current_code < LZWS_ALPHABET_LENGTH && need_to_clear_by_ratio_wrapper) {
    state_ptr->next_symbol  = (lzws_byte_fast_t) current_code;
    state_ptr->current_code = LZWS_CLEAR_CODE;
    state_ptr->status       = LZWS_COMPRESSOR_WRITE_CURRENT_CODE;

    return 0;
  }

  lzws_byte_fast_t symbol;
  lzws_compressor_read_byte(state_ptr, &symbol, source_ptr, source_length_ptr);

  lzws_code_fast_t next_code = lzws_compressor_get_next_code_from_dictionary_wrapper(state_ptr, current_code, symbol);
  if (next_code != LZWS_COMPRESSOR_UNDEFINED_NEXT_CODE) {
    // We found next code, it becomes new current code.
    state_ptr->current_code = next_code;

    // It is possible to keep next symbol in state as is.
    // Algorithm won't touch it without reinitialization.

    // We don't need to change status, algorithm wants next symbol.
    return 0;
  }

  // We can't find next code, we need to write current code.

  // We need to store symbol for future use.
  state_ptr->next_symbol = symbol;

  // We need to check whether we need to write alignment (there will be at least one code after it).
  if (lzws_compressor_need_to_write_alignment_wrapper(state_ptr)) {
    state_ptr->status = LZWS_COMPRESSOR_WRITE_REMAINDER_BEFORE_CURRENT_CODE;
  } else {
    state_ptr->status = LZWS_COMPRESSOR_WRITE_CURRENT_CODE;
  }

  return 0;
}

LZWS_EXPORT void lzws_compressor_process_eof_before_next_symbol(lzws_compressor_state_t* state_ptr)
{
  // We need to check whether we need to write alignment (there will be one code after it).
  if (lzws_compressor_need_to_write_alignment_wrapper(state_ptr)) {
    state_ptr->status = LZWS_COMPRESSOR_FLUSH_REMAINDER_BEFORE_CURRENT_CODE;
  } else {
    state_ptr->status = LZWS_COMPRESSOR_FLUSH_CURRENT_CODE;
  }
}
