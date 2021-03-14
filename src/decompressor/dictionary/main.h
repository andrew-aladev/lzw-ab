// LZW streaming decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_DECOMPRESSOR_DICTIONARY_MAIN_H)
#define LZWS_DECOMPRESSOR_DICTIONARY_MAIN_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

LZWS_EXPORT void lzws_decompressor_initialize_dictionary(lzws_decompressor_dictionary_t* dictionary_ptr);

LZWS_EXPORT lzws_result_t lzws_decompressor_allocate_dictionary(
  lzws_decompressor_dictionary_t*    dictionary_ptr,
  size_t                             total_codes_length,
  lzws_code_fast_t                   first_free_code,
  const lzws_decompressor_options_t* options_ptr);

LZWS_EXPORT void lzws_decompressor_write_code_to_dictionary(
  lzws_decompressor_dictionary_t* dictionary_ptr,
  lzws_code_fast_t                code);

LZWS_EXPORT void lzws_decompressor_add_code_to_dictionary(
  lzws_decompressor_dictionary_t* dictionary_ptr,
  lzws_code_fast_t                prefix_code,
  lzws_code_fast_t                current_code,
  lzws_code_fast_t                next_code);

LZWS_EXPORT inline bool lzws_decompressor_has_symbol_in_dictionary(const lzws_decompressor_dictionary_t* dictionary_ptr)
{
  return dictionary_ptr->output_length != 0;
}

LZWS_EXPORT inline lzws_byte_t lzws_decompressor_get_symbol_from_dictionary(
  lzws_decompressor_dictionary_t* dictionary_ptr)
{
  return dictionary_ptr->output_buffer[--dictionary_ptr->output_length];
}

LZWS_EXPORT void lzws_decompressor_free_dictionary(lzws_decompressor_dictionary_t* dictionary_ptr);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LZWS_DECOMPRESSOR_DICTIONARY_MAIN_H
