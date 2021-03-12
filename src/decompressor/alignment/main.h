// LZW streaming decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_DECOMPRESSOR_ALIGNMENT_MAIN_H)
#define LZWS_DECOMPRESSOR_ALIGNMENT_MAIN_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

inline void lzws_decompressor_initialize_alignment(
  lzws_decompressor_alignment_t* alignment_ptr,
  lzws_byte_fast_t               free_code_bit_length)
{
  alignment_ptr->free_code_bit_length = free_code_bit_length;
  alignment_ptr->source_byte_length   = 0;
}

inline void lzws_decompressor_add_source_byte_length_to_alignment(
  lzws_decompressor_alignment_t* alignment_ptr,
  lzws_byte_fast_t               source_byte_length)
{
  // Source byte length can be aligned by free code bit length * 8.
  alignment_ptr->source_byte_length =
    (alignment_ptr->source_byte_length + source_byte_length) % alignment_ptr->free_code_bit_length;
}

inline bool lzws_decompressor_need_to_read_alignment(
  const lzws_decompressor_alignment_t* alignment_ptr,
  lzws_byte_fast_t                     free_code_bit_length)
{
  return alignment_ptr->free_code_bit_length != free_code_bit_length;
}

inline bool lzws_decompressor_need_to_read_alignment_byte(const lzws_decompressor_alignment_t* alignment_ptr)
{
  return alignment_ptr->source_byte_length != 0;
}

inline void lzws_decompressor_reset_alignment_after_reading(
  lzws_decompressor_alignment_t* alignment_ptr,
  lzws_byte_fast_t               free_code_bit_length)
{
  alignment_ptr->free_code_bit_length = free_code_bit_length;
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LZWS_DECOMPRESSOR_ALIGNMENT_MAIN_H
