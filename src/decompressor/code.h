// LZW streaming decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_DECOMPRESSOR_CODE_H)
#define LZWS_DECOMPRESSOR_CODE_H

#include "state.h"

#ifdef __cplusplus
extern "C" {
#endif

lzws_result_t lzws_decompressor_read_code(
  lzws_decompressor_state_t* state_ptr,
  lzws_code_fast_t*          code_ptr,
  lzws_byte_t**              source_ptr,
  size_t*                    source_length_ptr);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LZWS_DECOMPRESSOR_CODE_H
