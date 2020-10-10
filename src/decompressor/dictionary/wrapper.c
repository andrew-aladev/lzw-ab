// LZW streaming decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "wrapper.h"

extern inline void          lzws_decompressor_initialize_dictionary_wrapper(lzws_decompressor_state_t* state_ptr);
extern inline lzws_result_t lzws_decompressor_allocate_dictionary_wrapper(lzws_decompressor_state_t* state_ptr);
extern inline void          lzws_decompressor_write_code_to_dictionary_wrapper(
           lzws_decompressor_state_t* state_ptr,
           lzws_code_fast_t           code);
extern inline void lzws_decompressor_add_code_to_dictionary_wrapper(
  lzws_decompressor_state_t* state_ptr,
  lzws_code_fast_t           prefix_code,
  lzws_code_fast_t           current_code,
  lzws_code_fast_t           next_code);
extern inline bool lzws_decompressor_has_symbol_in_dictionary_wrapper(const lzws_decompressor_state_t* state_ptr);
extern inline lzws_byte_t lzws_decompressor_get_symbol_from_dictionary_wrapper(lzws_decompressor_state_t* state_ptr);
extern inline void        lzws_decompressor_free_dictionary_wrapper(lzws_decompressor_state_t* state_ptr);
