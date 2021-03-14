// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "main.h"

LZWS_EXPORT extern inline void lzws_compressor_initialize_alignment(
  lzws_compressor_alignment_t* alignment_ptr,
  lzws_byte_fast_t             last_used_code_bit_length);

LZWS_EXPORT extern inline void lzws_compressor_add_destination_byte_length_to_alignment(
  lzws_compressor_alignment_t* alignment_ptr,
  lzws_byte_fast_t             destination_byte_length);

LZWS_EXPORT extern inline bool lzws_compressor_need_to_write_alignment(
  const lzws_compressor_alignment_t* alignment_ptr,
  lzws_byte_fast_t                   last_used_code_bit_length);

LZWS_EXPORT extern inline bool lzws_compressor_need_to_write_alignment_byte(
  const lzws_compressor_alignment_t* alignment_ptr);

LZWS_EXPORT extern inline void lzws_compressor_reset_alignment_after_writing(
  lzws_compressor_alignment_t* alignment_ptr,
  lzws_byte_fast_t             last_used_code_bit_length);
