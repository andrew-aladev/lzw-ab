// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "wrapper.h"

extern inline void lzws_compressor_initialize_dictionary_wrapper(lzws_compressor_state_t* state_ptr);

lzws_result_t lzws_compressor_allocate_dictionary_wrapper(lzws_compressor_state_t* state_ptr)
{
  size_t total_codes_length = lzws_compressor_get_total_codes_length(state_ptr);

  lzws_result_t result =
    lzws_compressor_allocate_dictionary(&state_ptr->dictionary, total_codes_length, &state_ptr->options);

  if (result != 0) {
    return result;
  }

  state_ptr->status = LZWS_COMPRESSOR_READ_FIRST_SYMBOL;

  return 0;
}

extern inline void lzws_compressor_clear_dictionary_wrapper(lzws_compressor_state_t* state_ptr);

extern inline lzws_code_fast_t lzws_compressor_get_next_code_from_dictionary_wrapper(
  const lzws_compressor_state_t* state_ptr,
  lzws_code_fast_t               current_code,
  lzws_byte_fast_t               next_symbol);

extern inline void lzws_compressor_save_next_code_to_dictionary_wrapper(
  lzws_compressor_state_t* state_ptr,
  lzws_code_fast_t         current_code,
  lzws_byte_fast_t         next_symbol,
  lzws_code_fast_t         next_code);

extern inline void lzws_compressor_free_dictionary_wrapper(lzws_compressor_state_t* state_ptr);
