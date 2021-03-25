// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_COMPRESSOR_SYMBOL_H)
#define LZWS_COMPRESSOR_SYMBOL_H

#include "state.h"

#if defined(__cplusplus)
extern "C" {
#endif

LZWS_EXPORT lzws_result_t lzws_compressor_read_first_symbol(
  lzws_compressor_state_t* state_ptr,
  lzws_byte_t**            source_ptr,
  size_t*                  source_length_ptr);

LZWS_EXPORT lzws_result_t lzws_compressor_read_next_symbol(
  lzws_compressor_state_t* state_ptr,
  lzws_byte_t**            source_ptr,
  size_t*                  source_length_ptr);

LZWS_EXPORT void lzws_compressor_process_eof_before_next_symbol(lzws_compressor_state_t* state_ptr);

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // LZWS_COMPRESSOR_SYMBOL_H
