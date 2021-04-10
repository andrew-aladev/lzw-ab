// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_COMPRESSOR_ALIGNMENT_MAIN_H)
#define LZWS_COMPRESSOR_ALIGNMENT_MAIN_H

#include "common.h"

#if defined(__cplusplus)
extern "C" {
#endif

LZWS_EXPORT inline void lzws_compressor_initialize_alignment(
  lzws_compressor_alignment_t* alignment_ptr,
  lzws_byte_fast_t             last_used_code_bit_length)
{
  alignment_ptr->last_used_code_bit_length = last_used_code_bit_length;
  alignment_ptr->destination_byte_length   = 0;
}

LZWS_EXPORT inline void lzws_compressor_add_destination_byte_length_to_alignment(
  lzws_compressor_alignment_t* alignment_ptr,
  lzws_byte_fast_t             destination_byte_length)
{
  // Destination byte length can be aligned by last used code bit length * 8.
  alignment_ptr->destination_byte_length = (lzws_byte_fast_t)(
    (alignment_ptr->destination_byte_length + destination_byte_length) % alignment_ptr->last_used_code_bit_length);
}

LZWS_EXPORT inline bool lzws_compressor_need_to_write_alignment(
  const lzws_compressor_alignment_t* alignment_ptr,
  lzws_byte_fast_t                   last_used_code_bit_length)
{
  return alignment_ptr->last_used_code_bit_length != last_used_code_bit_length;
}

LZWS_EXPORT inline bool lzws_compressor_need_to_write_alignment_byte(const lzws_compressor_alignment_t* alignment_ptr)
{
  return alignment_ptr->destination_byte_length != 0;
}

LZWS_EXPORT inline void lzws_compressor_reset_alignment_after_writing(
  lzws_compressor_alignment_t* alignment_ptr,
  lzws_byte_fast_t             last_used_code_bit_length)
{
  alignment_ptr->last_used_code_bit_length = last_used_code_bit_length;
}

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // LZWS_COMPRESSOR_ALIGNMENT_MAIN_H
