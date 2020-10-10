// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "utils.h"

extern inline lzws_byte_fast_t lzws_get_byte_with_reversed_bits(lzws_byte_fast_t byte);
extern inline lzws_code_fast_t lzws_get_mask_for_last_bits(lzws_byte_fast_t bit_length);
extern inline lzws_code_fast_t lzws_get_max_value_for_bits(lzws_byte_fast_t bit_length);
extern inline size_t           lzws_floor_bit_length_to_byte_length(size_t bit_length);
extern inline size_t           lzws_ceil_bit_length_to_byte_length(size_t bit_length);

extern inline void lzws_read_byte(lzws_byte_fast_t* byte_ptr, lzws_byte_t** source_ptr, size_t* source_length_ptr);
extern inline void
  lzws_write_byte(lzws_byte_fast_t byte, lzws_byte_t** destination_ptr, size_t* destination_length_ptr);
extern inline void
  lzws_fill_array(void* array, size_t size_of_item, size_t length, const void* item_ptr, bool item_bytes_are_identical);

extern inline void* lzws_allocate_array(
  lzws_byte_fast_t size_of_item,
  size_t           length,
  const void*      item_ptr,
  bool             item_is_zero,
  bool             item_bytes_are_identical);
