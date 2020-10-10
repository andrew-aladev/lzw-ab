// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "main.h"

#include "../../../log.h"
#include "../../../utils.h"
#include "../../common.h"

// -- codes length --

static inline size_t get_first_child_codes_length(
  const lzws_compressor_dictionary_t* dictionary_ptr,
  size_t                              total_codes_length)
{
  return total_codes_length - dictionary_ptr->first_child_codes_offset;
}

static inline size_t get_next_sibling_codes_length(
  const lzws_compressor_dictionary_t* dictionary_ptr,
  size_t                              total_codes_length)
{
  return total_codes_length - dictionary_ptr->next_sibling_codes_offset;
}

// -- code index --

static inline lzws_code_fast_t get_first_child_code_index(
  const lzws_compressor_dictionary_t* dictionary_ptr,
  lzws_code_fast_t                    first_free_code,
  lzws_code_fast_t                    code)
{
  if (code >= first_free_code) {
    code -= dictionary_ptr->first_child_codes_offset;
  }

  return code;
}

static inline lzws_code_fast_t get_next_sibling_code_index(
  const lzws_compressor_dictionary_t* dictionary_ptr,
  lzws_code_fast_t                    code)
{
  return code - dictionary_ptr->next_sibling_codes_offset;
}

// -- implementation --

extern inline void lzws_compressor_initialize_dictionary(
  lzws_compressor_dictionary_t* dictionary_ptr,
  lzws_code_fast_t              first_free_code);

lzws_result_t lzws_compressor_allocate_dictionary(
  lzws_compressor_dictionary_t*    dictionary_ptr,
  size_t                           total_codes_length,
  const lzws_compressor_options_t* options)
{
  lzws_code_t undefined_next_code = LZWS_COMPRESSOR_UNDEFINED_NEXT_CODE;

  size_t first_child_codes_length = get_first_child_codes_length(dictionary_ptr, total_codes_length);
  size_t first_child_codes_size   = sizeof(lzws_code_t) * first_child_codes_length;

  lzws_code_t* first_child_codes = lzws_allocate_array(
    sizeof(lzws_code_t),
    first_child_codes_length,
    &undefined_next_code,
    undefined_next_code == 0,
    LZWS_COMPRESSOR_UNDEFINED_NEXT_CODE_HAS_IDENTICAL_BYTES);

  if (first_child_codes == NULL) {
    if (!options->quiet) {
      LZWS_LOG_ERROR("allocate array failed, first child codes size: %zu", first_child_codes_size);
    }

    return LZWS_COMPRESSOR_ALLOCATE_FAILED;
  }

  size_t next_sibling_codes_length = get_next_sibling_codes_length(dictionary_ptr, total_codes_length);
  size_t next_sibling_codes_size   = sizeof(lzws_code_t) * next_sibling_codes_length;

  lzws_code_t* next_sibling_codes = lzws_allocate_array(
    sizeof(lzws_code_t),
    next_sibling_codes_length,
    &undefined_next_code,
    undefined_next_code == 0,
    LZWS_COMPRESSOR_UNDEFINED_NEXT_CODE_HAS_IDENTICAL_BYTES);

  if (next_sibling_codes == NULL) {
    if (!options->quiet) {
      LZWS_LOG_ERROR("allocate array failed, next sibling codes size: %zu", next_sibling_codes_size);
    }

    // First child codes was allocated, need to free it.
    free(first_child_codes);

    return LZWS_COMPRESSOR_ALLOCATE_FAILED;
  }

  // Last symbol by codes don't require default values.
  // Algorithm will access only initialized symbols.

  size_t last_symbol_by_codes_size = next_sibling_codes_length;

  lzws_byte_t* last_symbol_by_codes = malloc(last_symbol_by_codes_size);
  if (last_symbol_by_codes == NULL) {
    if (!options->quiet) {
      LZWS_LOG_ERROR("malloc failed, last symbol by codes size: %zu", last_symbol_by_codes_size);
    }

    // First child codes and next sibling codes were allocated, need to free it.
    free(first_child_codes);
    free(next_sibling_codes);

    return LZWS_COMPRESSOR_ALLOCATE_FAILED;
  }

  dictionary_ptr->first_child_codes    = first_child_codes;
  dictionary_ptr->next_sibling_codes   = next_sibling_codes;
  dictionary_ptr->last_symbol_by_codes = last_symbol_by_codes;

  return 0;
}

void lzws_compressor_clear_dictionary(lzws_compressor_dictionary_t* dictionary_ptr, size_t total_codes_length)
{
  lzws_code_t undefined_next_code = LZWS_COMPRESSOR_UNDEFINED_NEXT_CODE;

  size_t first_child_codes_length = get_first_child_codes_length(dictionary_ptr, total_codes_length);
  lzws_fill_array(
    dictionary_ptr->first_child_codes,
    sizeof(lzws_code_t),
    first_child_codes_length,
    &undefined_next_code,
    LZWS_COMPRESSOR_UNDEFINED_NEXT_CODE_HAS_IDENTICAL_BYTES);

  size_t next_sibling_codes_length = get_next_sibling_codes_length(dictionary_ptr, total_codes_length);
  lzws_fill_array(
    dictionary_ptr->next_sibling_codes,
    sizeof(lzws_code_t),
    next_sibling_codes_length,
    &undefined_next_code,
    LZWS_COMPRESSOR_UNDEFINED_NEXT_CODE_HAS_IDENTICAL_BYTES);

  // We can keep last symbol by codes in dictionary as is.
  // Algorithm will access only initialized symbols.
}

lzws_code_fast_t lzws_compressor_get_next_code_from_dictionary(
  const lzws_compressor_dictionary_t* dictionary_ptr,
  lzws_code_fast_t                    first_free_code,
  lzws_code_fast_t                    current_code,
  lzws_byte_fast_t                    next_symbol)
{
  lzws_code_fast_t current_code_index = get_first_child_code_index(dictionary_ptr, first_free_code, current_code);

  lzws_code_t* next_sibling_codes   = dictionary_ptr->next_sibling_codes;
  lzws_byte_t* last_symbol_by_codes = dictionary_ptr->last_symbol_by_codes;

  lzws_code_fast_t next_sibling_code = dictionary_ptr->first_child_codes[current_code_index];
  lzws_code_fast_t next_sibling_code_index;

  while (next_sibling_code != LZWS_COMPRESSOR_UNDEFINED_NEXT_CODE) {
    next_sibling_code_index = get_next_sibling_code_index(dictionary_ptr, next_sibling_code);
    if (last_symbol_by_codes[next_sibling_code_index] == next_symbol) {
      // We found target next symbol.
      return next_sibling_code;
    }

    next_sibling_code = next_sibling_codes[next_sibling_code_index];
  }

  // We can't find next symbol.
  return LZWS_COMPRESSOR_UNDEFINED_NEXT_CODE;
}

void lzws_compressor_save_next_code_to_dictionary(
  lzws_compressor_dictionary_t* dictionary_ptr,
  lzws_code_fast_t              first_free_code,
  lzws_code_fast_t              current_code,
  lzws_byte_fast_t              next_symbol,
  lzws_code_fast_t              next_code)
{
  lzws_code_fast_t next_code_index = get_next_sibling_code_index(dictionary_ptr, next_code);

  // We need to store next symbol for next code.
  dictionary_ptr->last_symbol_by_codes[next_code_index] = next_symbol;

  lzws_code_fast_t current_code_index = get_first_child_code_index(dictionary_ptr, first_free_code, current_code);

  lzws_code_t*     first_child_codes = dictionary_ptr->first_child_codes;
  lzws_code_fast_t first_child_code  = first_child_codes[current_code_index];

  // Saving first child.
  first_child_codes[current_code_index] = next_code;

  if (first_child_code != LZWS_COMPRESSOR_UNDEFINED_NEXT_CODE) {
    // Saving next sibling.
    dictionary_ptr->next_sibling_codes[next_code_index] = first_child_code;
  }
}

extern inline void lzws_compressor_free_dictionary(lzws_compressor_dictionary_t* dictionary_ptr);
