// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_COMPRESSOR_DICTIONARY_LINKED_LIST_MAIN_H)
#define LZWS_COMPRESSOR_DICTIONARY_LINKED_LIST_MAIN_H

#include <stdlib.h>

#include "common.h"

inline void lzws_compressor_initialize_dictionary(
  lzws_compressor_dictionary_t* dictionary_ptr,
  lzws_code_fast_t              first_free_code)
{
  dictionary_ptr->first_child_codes    = NULL;
  dictionary_ptr->next_sibling_codes   = NULL;
  dictionary_ptr->last_symbol_by_codes = NULL;

  // We won't store clear code.
  dictionary_ptr->first_child_codes_offset = first_free_code - LZWS_ALPHABET_LENGTH;

  // We won't store char codes and clear code.
  dictionary_ptr->next_sibling_codes_offset = first_free_code;
}

lzws_result_t lzws_compressor_allocate_dictionary(
  lzws_compressor_dictionary_t*    dictionary_ptr,
  size_t                           total_codes_length,
  const lzws_compressor_options_t* options);
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

inline void lzws_compressor_free_dictionary(lzws_compressor_dictionary_t* dictionary_ptr)
{
  lzws_code_t* first_child_codes = dictionary_ptr->first_child_codes;
  if (first_child_codes != NULL) {
    free(first_child_codes);
  }

  lzws_code_t* next_sibling_codes = dictionary_ptr->next_sibling_codes;
  if (next_sibling_codes != NULL) {
    free(next_sibling_codes);
  }

  lzws_byte_t* last_symbol_by_codes = dictionary_ptr->last_symbol_by_codes;
  if (last_symbol_by_codes != NULL) {
    free(last_symbol_by_codes);
  }
}

#endif // LZWS_COMPRESSOR_DICTIONARY_LINKED_LIST_MAIN_H
