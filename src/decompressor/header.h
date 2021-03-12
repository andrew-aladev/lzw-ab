// LZW streaming decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_DECOMPRESSOR_HEADER_H)
#define LZWS_DECOMPRESSOR_HEADER_H

#include "state.h"

#ifdef __cplusplus
extern "C" {
#endif

// This function is optional.
// Use it to be compatible with original UNIX compress utility.
lzws_result_t lzws_decompressor_read_magic_header(
  lzws_decompressor_state_t* state_ptr,
  lzws_byte_t**              source_ptr,
  size_t*                    source_length_ptr);

lzws_result_t lzws_decompressor_read_header(
  lzws_decompressor_state_t* state_ptr,
  lzws_byte_t**              source_ptr,
  size_t*                    source_length_ptr);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LZWS_DECOMPRESSOR_HEADER_H
