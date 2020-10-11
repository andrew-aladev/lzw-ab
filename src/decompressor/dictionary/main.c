// LZW streaming decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "main.h"

#include "../../log.h"

// -- codes length --

static inline size_t get_codes_length(const lzws_decompressor_dictionary_t* dictionary_ptr, size_t total_codes_length)
{
  return total_codes_length - dictionary_ptr->codes_offset;
}

// -- code index --

static inline lzws_code_fast_t get_code_index(
  const lzws_decompressor_dictionary_t* dictionary_ptr,
  lzws_code_fast_t                      code)
{
  return code - dictionary_ptr->codes_offset;
}

// -- implementation --

void lzws_decompressor_initialize_dictionary(lzws_decompressor_dictionary_t* dictionary_ptr)
{
  dictionary_ptr->previous_codes       = NULL;
  dictionary_ptr->last_symbol_by_codes = NULL;
  dictionary_ptr->output_buffer        = NULL;

  // It is possible to keep output length uninitialized.
  // Other data will be initialized during allocating of dictionary.
}

lzws_result_t lzws_decompressor_allocate_dictionary(
  lzws_decompressor_dictionary_t*    dictionary_ptr,
  size_t                             total_codes_length,
  lzws_code_fast_t                   first_free_code,
  const lzws_decompressor_options_t* options_ptr)
{
  // We won't store char codes and clear code.
  dictionary_ptr->codes_offset = first_free_code;

  size_t codes_length = get_codes_length(dictionary_ptr, total_codes_length);

  // Previous codes don't require default values.
  // Algorithm will access only initialized codes.

  size_t previous_codes_size = codes_length * sizeof(lzws_code_t);

  lzws_code_t* previous_codes = malloc(previous_codes_size);
  if (previous_codes == NULL) {
    if (!options_ptr->quiet) {
      LZWS_LOG_ERROR("malloc failed, previous codes size: %zu", previous_codes_size);
    }

    return LZWS_DECOMPRESSOR_ALLOCATE_FAILED;
  }

  // Last symbol by codes don't require default values.
  // Algorithm will access only initialized symbols.

  size_t last_symbol_by_codes_size = codes_length;

  lzws_byte_t* last_symbol_by_codes = malloc(last_symbol_by_codes_size);
  if (last_symbol_by_codes == NULL) {
    if (!options_ptr->quiet) {
      LZWS_LOG_ERROR("malloc failed, last symbol by codes size: %zu", last_symbol_by_codes_size);
    }

    // Previous codes was allocated, need to free it.
    free(previous_codes);
    return LZWS_DECOMPRESSOR_ALLOCATE_FAILED;
  }

  // Output buffer by codes don't require default values.
  // Algorithm will access only initialized buffer data.

  // + 1 for symbol from prefix.
  size_t output_size = codes_length + 1;

  lzws_byte_t* output_buffer = malloc(output_size);
  if (output_buffer == NULL) {
    if (!options_ptr->quiet) {
      LZWS_LOG_ERROR("malloc failed, output size: %zu", output_size);
    }

    // Previous codes and last symbol by codes were allocated, need to free it.
    free(previous_codes);
    free(last_symbol_by_codes);
    return LZWS_DECOMPRESSOR_ALLOCATE_FAILED;
  }

  dictionary_ptr->previous_codes       = previous_codes;
  dictionary_ptr->last_symbol_by_codes = last_symbol_by_codes;
  dictionary_ptr->output_buffer        = output_buffer;

  return 0;
}

static inline lzws_byte_t
  prepare_output(lzws_decompressor_dictionary_t* dictionary_ptr, lzws_code_fast_t code, bool is_prefix)
{
  // First symbol equals last symbol when code is a prefix.
  // Output buffer is reversed.
  // So we need to copy last symbol into the first position.

  size_t output_length;
  if (is_prefix) {
    output_length = 1;
  } else {
    output_length = 0;
  }

  lzws_code_fast_t   codes_offset         = dictionary_ptr->codes_offset;
  const lzws_code_t* previous_codes       = dictionary_ptr->previous_codes;
  const lzws_byte_t* last_symbol_by_codes = dictionary_ptr->last_symbol_by_codes;
  lzws_byte_t*       output_buffer        = dictionary_ptr->output_buffer;
  lzws_code_fast_t   code_index;

  // It is not possible to receive clear code during code sequence.
  // So we need to compare code with codes offset.

  while (code >= codes_offset) {
    code_index = get_code_index(dictionary_ptr, code);

    output_buffer[output_length] = last_symbol_by_codes[code_index];
    output_length++;

    code = previous_codes[code_index];
  }

  lzws_byte_t first_symbol = code;

  output_buffer[output_length] = first_symbol;
  output_length++;

  if (is_prefix) {
    // Last symbol equals to first symbol.
    output_buffer[0] = first_symbol;
  }

  dictionary_ptr->output_length = output_length;

  return first_symbol;
}

void lzws_decompressor_write_code_to_dictionary(lzws_decompressor_dictionary_t* dictionary_ptr, lzws_code_fast_t code)
{
  prepare_output(dictionary_ptr, code, false);
}

// Example: (ab)(cde).
// Prefix code - ab, current code - cde, next code - abc.
// We can see that last symbol of next code equals to first symbol of current code.

void lzws_decompressor_add_code_to_dictionary(
  lzws_decompressor_dictionary_t* dictionary_ptr,
  lzws_code_fast_t                prefix_code,
  lzws_code_fast_t                current_code,
  lzws_code_fast_t                next_code)
{
  lzws_code_fast_t code;

  bool is_prefix = current_code == next_code;
  if (is_prefix) {
    code = prefix_code;
  } else {
    code = current_code;
  }

  lzws_code_fast_t next_code_index = get_code_index(dictionary_ptr, next_code);
  lzws_byte_t      first_symbol    = prepare_output(dictionary_ptr, code, is_prefix);

  dictionary_ptr->previous_codes[next_code_index]       = prefix_code;
  dictionary_ptr->last_symbol_by_codes[next_code_index] = first_symbol;
}

extern inline bool lzws_decompressor_has_symbol_in_dictionary(const lzws_decompressor_dictionary_t* dictionary_ptr);
extern inline lzws_byte_t lzws_decompressor_get_symbol_from_dictionary(lzws_decompressor_dictionary_t* dictionary_ptr);

void lzws_decompressor_free_dictionary(lzws_decompressor_dictionary_t* dictionary_ptr)
{
  lzws_code_t* previous_codes = dictionary_ptr->previous_codes;
  if (previous_codes != NULL) {
    free(previous_codes);
  }

  lzws_byte_t* last_symbol_by_codes = dictionary_ptr->last_symbol_by_codes;
  if (last_symbol_by_codes != NULL) {
    free(last_symbol_by_codes);
  }

  lzws_byte_t* output_buffer = dictionary_ptr->output_buffer;
  if (output_buffer != NULL) {
    free(output_buffer);
  }
}
