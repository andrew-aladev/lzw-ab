// LZW streaming decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "main.h"

extern inline void lzws_decompressor_initialize_alignment(lzws_decompressor_alignment_t* alignment_ptr, lzws_byte_fast_t free_code_bit_length);
extern inline void lzws_decompressor_add_source_byte_length_to_alignment(lzws_decompressor_alignment_t* alignment_ptr, lzws_byte_fast_t source_byte_length);
extern inline bool lzws_decompressor_need_to_read_alignment(const lzws_decompressor_alignment_t* alignment_ptr, lzws_byte_fast_t free_code_bit_length);
extern inline bool lzws_decompressor_need_to_read_alignment_byte(const lzws_decompressor_alignment_t* alignment_ptr);
extern inline void lzws_decompressor_reset_alignment_after_reading(lzws_decompressor_alignment_t* alignment_ptr, lzws_byte_fast_t free_code_bit_length);
