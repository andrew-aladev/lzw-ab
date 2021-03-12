// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_COMPRESSOR_DICTIONARY_LINKED_LIST_WRAPPER_H)
#define LZWS_COMPRESSOR_DICTIONARY_LINKED_LIST_WRAPPER_H

#include "../../state.h"
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

inline void lzws_compressor_initialize_dictionary_wrapper(lzws_compressor_state_t* state_ptr)
{
  lzws_compressor_initialize_dictionary(&state_ptr->dictionary, state_ptr->first_free_code);
}

lzws_result_t lzws_compressor_allocate_dictionary_wrapper(lzws_compressor_state_t* state_ptr);

inline void lzws_compressor_clear_dictionary_wrapper(lzws_compressor_state_t* state_ptr)
{
  if (state_ptr->options.block_mode) {
    size_t total_codes_length = lzws_compressor_get_total_codes_length(state_ptr);
    lzws_compressor_clear_dictionary(&state_ptr->dictionary, total_codes_length);
  }
}

inline lzws_code_fast_t lzws_compressor_get_next_code_from_dictionary_wrapper(
  const lzws_compressor_state_t* state_ptr,
  lzws_code_fast_t               current_code,
  lzws_byte_fast_t               next_symbol)
{
  return lzws_compressor_get_next_code_from_dictionary(
    &state_ptr->dictionary, state_ptr->first_free_code, current_code, next_symbol);
}

inline void lzws_compressor_save_next_code_to_dictionary_wrapper(
  lzws_compressor_state_t* state_ptr,
  lzws_code_fast_t         current_code,
  lzws_byte_fast_t         next_symbol,
  lzws_code_fast_t         next_code)
{
  lzws_compressor_save_next_code_to_dictionary(
    &state_ptr->dictionary, state_ptr->first_free_code, current_code, next_symbol, next_code);
}

inline void lzws_compressor_free_dictionary_wrapper(lzws_compressor_state_t* state_ptr)
{
  lzws_compressor_free_dictionary(&state_ptr->dictionary);
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LZWS_COMPRESSOR_DICTIONARY_LINKED_LIST_WRAPPER_H
