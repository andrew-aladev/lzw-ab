// LZW streaming decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_DECOMPRESSOR_DICTIONARY_WRAPPER_H)
#define LZWS_DECOMPRESSOR_DICTIONARY_WRAPPER_H

#include "../state.h"
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

inline void lzws_decompressor_initialize_dictionary_wrapper(lzws_decompressor_state_t* state_ptr)
{
  lzws_decompressor_initialize_dictionary(&state_ptr->dictionary);
}

LZWS_EXPORT lzws_result_t lzws_decompressor_allocate_dictionary_wrapper(lzws_decompressor_state_t* state_ptr);

inline void lzws_decompressor_write_code_to_dictionary_wrapper(
  lzws_decompressor_state_t* state_ptr,
  lzws_code_fast_t           code)
{
  lzws_decompressor_write_code_to_dictionary(&state_ptr->dictionary, code);
}

inline void lzws_decompressor_add_code_to_dictionary_wrapper(
  lzws_decompressor_state_t* state_ptr,
  lzws_code_fast_t           prefix_code,
  lzws_code_fast_t           current_code,
  lzws_code_fast_t           next_code)
{
  lzws_decompressor_add_code_to_dictionary(&state_ptr->dictionary, prefix_code, current_code, next_code);
}

inline bool lzws_decompressor_has_symbol_in_dictionary_wrapper(const lzws_decompressor_state_t* state_ptr)
{
  return lzws_decompressor_has_symbol_in_dictionary(&state_ptr->dictionary);
}

inline lzws_byte_t lzws_decompressor_get_symbol_from_dictionary_wrapper(lzws_decompressor_state_t* state_ptr)
{
  return lzws_decompressor_get_symbol_from_dictionary(&state_ptr->dictionary);
}

inline void lzws_decompressor_free_dictionary_wrapper(lzws_decompressor_state_t* state_ptr)
{
  lzws_decompressor_free_dictionary(&state_ptr->dictionary);
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LZWS_DECOMPRESSOR_DICTIONARY_WRAPPER_H
