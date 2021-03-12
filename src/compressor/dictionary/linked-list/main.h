// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_COMPRESSOR_DICTIONARY_LINKED_LIST_MAIN_H)
#define LZWS_COMPRESSOR_DICTIONARY_LINKED_LIST_MAIN_H

#include <stdlib.h>

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

void lzws_compressor_initialize_dictionary(
  lzws_compressor_dictionary_t* dictionary_ptr,
  lzws_code_fast_t              first_free_code);

lzws_result_t lzws_compressor_allocate_dictionary(
  lzws_compressor_dictionary_t*    dictionary_ptr,
  size_t                           total_codes_length,
  const lzws_compressor_options_t* options_ptr);

void lzws_compressor_clear_dictionary(lzws_compressor_dictionary_t* dictionary_ptr, size_t total_codes_length);

lzws_code_fast_t lzws_compressor_get_next_code_from_dictionary(
  const lzws_compressor_dictionary_t* dictionary_ptr,
  lzws_code_fast_t                    first_free_code,
  lzws_code_fast_t                    current_code,
  lzws_byte_fast_t                    next_symbol);

void lzws_compressor_save_next_code_to_dictionary(
  lzws_compressor_dictionary_t* dictionary_ptr,
  lzws_code_fast_t              first_free_code,
  lzws_code_fast_t              current_code,
  lzws_byte_fast_t              next_symbol,
  lzws_code_fast_t              next_code);

void lzws_compressor_free_dictionary(lzws_compressor_dictionary_t* dictionary_ptr);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LZWS_COMPRESSOR_DICTIONARY_LINKED_LIST_MAIN_H
