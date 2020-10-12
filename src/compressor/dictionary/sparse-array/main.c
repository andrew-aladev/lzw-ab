// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "main.h"

#include "../../../log.h"
#include "../../../utils.h"

// -- codes length --

static inline size_t get_next_codes_length(
  const lzws_compressor_dictionary_t* dictionary_ptr,
  size_t                              total_codes_length)
{
  return (total_codes_length - dictionary_ptr->next_codes_offset) << LZWS_ALPHABET_BIT_LENGTH;
}

static inline size_t get_used_indexes_length(
  const lzws_compressor_dictionary_t* dictionary_ptr,
  size_t                              total_codes_length)
{
  return total_codes_length - dictionary_ptr->used_indexes_offset;
}

// -- code index --

static inline lzws_compressor_dictionary_used_index_t get_next_code_index(
  const lzws_compressor_dictionary_t* dictionary_ptr,
  lzws_code_fast_t                    first_free_code,
  lzws_code_fast_t                    current_code,
  lzws_byte_fast_t                    next_symbol)
{
  if (current_code >= first_free_code) {
    current_code -= dictionary_ptr->next_codes_offset;
  }

  return (current_code << LZWS_ALPHABET_BIT_LENGTH) | next_symbol;
}

static inline lzws_code_fast_t get_index_of_used_index(
  const lzws_compressor_dictionary_t* dictionary_ptr,
  lzws_code_fast_t                    code)
{
  return code - dictionary_ptr->used_indexes_offset;
}

// -- implementation --

void lzws_compressor_initialize_dictionary(
  lzws_compressor_dictionary_t*    dictionary_ptr,
  lzws_code_fast_t                 first_free_code,
  const lzws_compressor_options_t* options_ptr)
{
  dictionary_ptr->next_codes = NULL;

  // We won't store clear code.
  dictionary_ptr->next_codes_offset = first_free_code - LZWS_ALPHABET_LENGTH;

  if (options_ptr->block_mode) {
    dictionary_ptr->used_indexes = NULL;

    // We won't store char codes and clear code.
    dictionary_ptr->used_indexes_offset = first_free_code;
  }
}

extern inline void lzws_compressor_initialize_dictionary(
  lzws_compressor_dictionary_t*    dictionary_ptr,
  lzws_code_fast_t                 first_free_code,
  const lzws_compressor_options_t* options_ptr);

lzws_result_t lzws_compressor_allocate_dictionary(
  lzws_compressor_dictionary_t*    dictionary_ptr,
  size_t                           total_codes_length,
  const lzws_compressor_options_t* options_ptr)
{
  lzws_code_t undefined_next_code = LZWS_COMPRESSOR_UNDEFINED_NEXT_CODE;
  size_t      next_codes_length   = get_next_codes_length(dictionary_ptr, total_codes_length);
  size_t      next_codes_size     = sizeof(lzws_code_t) * next_codes_length;

  lzws_code_t* next_codes = lzws_allocate_array(
    sizeof(lzws_code_t),
    next_codes_length,
    &undefined_next_code,
    undefined_next_code == 0,
    LZWS_COMPRESSOR_UNDEFINED_NEXT_CODE_HAS_IDENTICAL_BYTES);

  if (next_codes == NULL) {
    if (!options_ptr->quiet) {
      LZWS_LOG_ERROR("allocate array failed, next codes size: %zu", next_codes_size);
    }

    return LZWS_COMPRESSOR_ALLOCATE_FAILED;
  }

  bool                                     block_mode = options_ptr->block_mode;
  lzws_compressor_dictionary_used_index_t* used_indexes;

  if (block_mode) {
    size_t used_indexes_length = get_used_indexes_length(dictionary_ptr, total_codes_length);

    // Used indexes don't require default values.
    // Algorithm will access only initialized indexes.

    size_t used_indexes_size = used_indexes_length * sizeof(lzws_compressor_dictionary_used_index_t);

    used_indexes = malloc(used_indexes_size);
    if (used_indexes == NULL) {
      if (!options_ptr->quiet) {
        LZWS_LOG_ERROR("malloc failed, used indexes size: %zu", used_indexes_size);
      }

      // Next codes was allocated, need to free it.
      free(next_codes);
      return LZWS_COMPRESSOR_ALLOCATE_FAILED;
    }
  }

  dictionary_ptr->next_codes = next_codes;

  if (block_mode) {
    dictionary_ptr->used_indexes = used_indexes;
  }

  return 0;
}

void lzws_compressor_clear_dictionary(lzws_compressor_dictionary_t* dictionary_ptr, size_t used_codes_length)
{
  lzws_code_t*                             next_codes   = dictionary_ptr->next_codes;
  lzws_compressor_dictionary_used_index_t* used_indexes = dictionary_ptr->used_indexes;
  size_t used_indexes_length                            = get_used_indexes_length(dictionary_ptr, used_codes_length);

  lzws_compressor_dictionary_used_index_t used_index;
  for (lzws_code_fast_t index = 0; index < used_indexes_length; index++) {
    used_index             = used_indexes[index];
    next_codes[used_index] = LZWS_COMPRESSOR_UNDEFINED_NEXT_CODE;
  }

  // We can keep used indexes in dictionary as is.
  // Algorithm will access only initialized indexes.
}

lzws_code_fast_t lzws_compressor_get_next_code_from_dictionary(
  const lzws_compressor_dictionary_t* dictionary_ptr,
  lzws_code_fast_t                    first_free_code,
  lzws_code_fast_t                    current_code,
  lzws_byte_fast_t                    next_symbol)
{
  lzws_compressor_dictionary_used_index_t code_index =
    get_next_code_index(dictionary_ptr, first_free_code, current_code, next_symbol);

  return dictionary_ptr->next_codes[code_index];
}

void lzws_compressor_save_next_code_to_dictionary(
  lzws_compressor_dictionary_t*    dictionary_ptr,
  lzws_code_fast_t                 first_free_code,
  const lzws_compressor_options_t* options_ptr,
  lzws_code_fast_t                 current_code,
  lzws_byte_fast_t                 next_symbol,
  lzws_code_fast_t                 next_code)
{
  lzws_compressor_dictionary_used_index_t code_index =
    get_next_code_index(dictionary_ptr, first_free_code, current_code, next_symbol);

  if (options_ptr->block_mode) {
    lzws_code_fast_t index              = get_index_of_used_index(dictionary_ptr, next_code);
    dictionary_ptr->used_indexes[index] = code_index;
  }

  dictionary_ptr->next_codes[code_index] = next_code;
}

void lzws_compressor_free_dictionary(
  lzws_compressor_dictionary_t*    dictionary_ptr,
  const lzws_compressor_options_t* options_ptr)
{
  lzws_code_t* next_codes = dictionary_ptr->next_codes;
  if (next_codes != NULL) {
    free(next_codes);
  }

  if (options_ptr->block_mode) {
    lzws_compressor_dictionary_used_index_t* used_indexes = dictionary_ptr->used_indexes;
    if (used_indexes != NULL) {
      free(used_indexes);
    }
  }
}
