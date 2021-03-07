// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "wrapper.h"

extern inline lzws_result_t lzws_compressor_initialize_ratio_wrapper(lzws_compressor_state_t* state_ptr);

extern inline void lzws_compressor_add_source_symbol_to_ratio_wrapper(lzws_compressor_state_t* state_ptr);
extern inline void lzws_compressor_add_destination_symbol_to_ratio_wrapper(lzws_compressor_state_t* state_ptr);

extern inline lzws_result_t lzws_compressor_need_to_clear_by_ratio_wrapper(
  lzws_compressor_state_t* state_ptr,
  bool*                    result_ptr);

extern inline lzws_result_t lzws_compressor_clear_ratio_wrapper(lzws_compressor_state_t* state_ptr);

extern inline void lzws_compressor_free_ratio_wrapper(lzws_compressor_state_t* state_ptr);
