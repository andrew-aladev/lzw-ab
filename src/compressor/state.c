// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "state.h"

#include "../log.h"
#include "../utils.h"
#include "alignment/wrapper.h"
#include "dictionary/wrapper.h"
#include "ratio/wrapper.h"

lzws_result_t lzws_compressor_get_initial_state(
  lzws_compressor_state_t**        result_state_ptr,
  const lzws_compressor_options_t* options)
{
  if (options == NULL) {
    options = &LZWS_COMPRESSOR_DEFAULT_OPTIONS;
  } else {
    if (
      options->max_code_bit_length < LZWS_LOWEST_MAX_CODE_BIT_LENGTH ||
      options->max_code_bit_length > LZWS_BIGGEST_MAX_CODE_BIT_LENGTH) {
      if (!options->quiet) {
        LZWS_LOG_ERROR("invalid max code bit length: %u", options->max_code_bit_length);
      }

      return LZWS_COMPRESSOR_INVALID_MAX_CODE_BIT_LENGTH;
    }
  }

  size_t state_size = sizeof(lzws_compressor_state_t);

  lzws_compressor_state_t* state_ptr = malloc(state_size);
  if (state_ptr == NULL) {
    if (!options->quiet) {
      LZWS_LOG_ERROR("malloc failed, state size: %zu", state_size);
    }

    return LZWS_COMPRESSOR_ALLOCATE_FAILED;
  }

  state_ptr->options = *options;
  state_ptr->status = options->without_magic_header ? LZWS_COMPRESSOR_WRITE_HEADER : LZWS_COMPRESSOR_WRITE_MAGIC_HEADER;

  state_ptr->first_free_code = lzws_get_first_free_code(options->block_mode);
  state_ptr->max_code        = lzws_get_max_value_for_bits(options->max_code_bit_length);

  lzws_compressor_reset_last_used_data(state_ptr);

  // It is possible to keep current code and next symbol uninitialized.

  state_ptr->remainder            = 0;
  state_ptr->remainder_bit_length = 0;

  lzws_compressor_initialize_alignment_wrapper(state_ptr);
  lzws_compressor_initialize_dictionary_wrapper(state_ptr);
  lzws_compressor_initialize_ratio_wrapper(state_ptr);

  *result_state_ptr = state_ptr;

  return 0;
}

void lzws_compressor_reset_last_used_data(lzws_compressor_state_t* state_ptr)
{
  state_ptr->last_used_code                    = state_ptr->first_free_code - 1;
  state_ptr->last_used_code_bit_length         = LZWS_LOWEST_MAX_CODE_BIT_LENGTH;
  state_ptr->max_last_used_code_for_bit_length = lzws_get_max_value_for_bits(LZWS_LOWEST_MAX_CODE_BIT_LENGTH);
}

void lzws_compressor_clear_state(lzws_compressor_state_t* state_ptr)
{
  lzws_compressor_clear_dictionary_wrapper(state_ptr);
  lzws_compressor_clear_ratio_wrapper(state_ptr);

  lzws_compressor_reset_last_used_data(state_ptr);
}

void lzws_compressor_free_state(lzws_compressor_state_t* state_ptr)
{
  lzws_compressor_free_dictionary_wrapper(state_ptr);
  lzws_compressor_free_ratio_wrapper(state_ptr);

  free(state_ptr);
}

extern inline bool   lzws_compressor_is_dictionary_full(const lzws_compressor_state_t* state_ptr);
extern inline size_t lzws_compressor_get_last_used_codes_length(const lzws_compressor_state_t* state_ptr);
extern inline size_t lzws_compressor_get_total_codes_length(const lzws_compressor_state_t* state_ptr);
