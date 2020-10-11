// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "utils.h"

#include <string.h>

extern inline lzws_byte_fast_t lzws_get_byte_with_reversed_bits(lzws_byte_fast_t byte);
extern inline lzws_code_fast_t lzws_get_mask_for_last_bits(lzws_byte_fast_t bit_length);
extern inline lzws_code_fast_t lzws_get_max_value_for_bits(lzws_byte_fast_t bit_length);
extern inline size_t           lzws_floor_bit_length_to_byte_length(size_t bit_length);
extern inline size_t           lzws_ceil_bit_length_to_byte_length(size_t bit_length);

extern inline void lzws_read_byte(lzws_byte_fast_t* byte_ptr, lzws_byte_t** source_ptr, size_t* source_length_ptr);
extern inline void
  lzws_write_byte(lzws_byte_fast_t byte, lzws_byte_t** destination_ptr, size_t* destination_length_ptr);

// Keeping universal fill and allocate functions despite the losses of coverage percent.

void lzws_fill_array(
  void*       array,
  size_t      size_of_item,
  size_t      length,
  const void* item_ptr,
  bool        item_bytes_are_identical)
{
  const lzws_byte_t* bytes      = item_ptr;
  lzws_byte_t        first_byte = bytes[0];

  if (item_bytes_are_identical) {
    memset(array, first_byte, size_of_item * length);
    return;
  }

  for (size_t index = 0; index < length; index++) {
    memcpy((lzws_byte_t*) array + size_of_item * index, item_ptr, size_of_item);
  }
}

void* lzws_allocate_array(
  lzws_byte_fast_t size_of_item,
  size_t           length,
  const void*      item_ptr,
  bool             item_is_zero,
  bool             item_bytes_are_identical)
{
  size_t size = size_of_item * length;
  if (item_is_zero) {
    return calloc(1, size);
  }

  void* array = malloc(size);
  if (array == NULL) {
    return NULL;
  }

  lzws_fill_array(array, size_of_item, length, item_ptr, item_bytes_are_identical);

  return array;
}
