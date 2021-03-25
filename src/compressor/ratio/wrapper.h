// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_COMPRESSOR_RATIO_WRAPPER_H)
#define LZWS_COMPRESSOR_RATIO_WRAPPER_H

#include "../state.h"
#include "main.h"

#if defined(__cplusplus)
extern "C" {
#endif

inline lzws_result_t lzws_compressor_initialize_ratio_wrapper(lzws_compressor_state_t* state_ptr)
{
  if (state_ptr->options.block_mode) {
    return lzws_compressor_initialize_ratio(&state_ptr->ratio, state_ptr->options.quiet);
  }

  return 0;
}

inline void lzws_compressor_add_source_symbol_to_ratio_wrapper(lzws_compressor_state_t* state_ptr)
{
  if (state_ptr->options.block_mode) {
    lzws_compressor_add_source_symbol_to_ratio(&state_ptr->ratio);
  }
}

inline void lzws_compressor_add_destination_symbol_to_ratio_wrapper(lzws_compressor_state_t* state_ptr)
{
  if (state_ptr->options.block_mode) {
    lzws_compressor_add_destination_symbol_to_ratio(&state_ptr->ratio);
  }
}

inline lzws_result_t lzws_compressor_need_to_clear_by_ratio_wrapper(
  lzws_compressor_state_t* state_ptr,
  bool*                    result_ptr)
{
  if (!state_ptr->options.block_mode || !lzws_compressor_is_dictionary_full(state_ptr)) {
    *result_ptr = false;
    return 0;
  }

  return lzws_compressor_need_to_clear_by_ratio(&state_ptr->ratio, state_ptr->options.quiet, result_ptr);
}

inline lzws_result_t lzws_compressor_clear_ratio_wrapper(lzws_compressor_state_t* state_ptr)
{
  if (state_ptr->options.block_mode) {
    return lzws_compressor_clear_ratio(&state_ptr->ratio, state_ptr->options.quiet);
  }

  return 0;
}

inline void lzws_compressor_free_ratio_wrapper(lzws_compressor_state_t* state_ptr)
{
  if (state_ptr->options.block_mode) {
    lzws_compressor_free_ratio(&state_ptr->ratio);
  }
}

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // LZWS_COMPRESSOR_RATIO_WRAPPER_H
