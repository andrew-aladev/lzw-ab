// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_UTILS_H)
#define LZWS_UTILS_H

#include <stdbool.h>
#include <stdlib.h>

#include "constants.h"

#if defined(__cplusplus)
extern "C" {
#endif

LZWS_EXPORT inline lzws_byte_fast_t lzws_get_byte_with_reversed_bits(lzws_byte_fast_t byte)
{
  return LZWS_BYTES_WITH_REVERSED_BITS[byte];
}

LZWS_EXPORT inline lzws_code_fast_t lzws_get_mask_for_last_bits(lzws_byte_fast_t bit_length)
{
  return LZWS_MASKS_FOR_LAST_BITS[bit_length];
}

LZWS_EXPORT inline lzws_code_fast_t lzws_get_max_value_for_bits(lzws_byte_fast_t bit_length)
{
  return lzws_get_mask_for_last_bits(bit_length);
}

LZWS_EXPORT inline size_t lzws_floor_bit_length_to_byte_length(size_t bit_length)
{
  return bit_length >> 3;
}

LZWS_EXPORT inline size_t lzws_ceil_bit_length_to_byte_length(size_t bit_length)
{
  return (bit_length + 7) >> 3;
}

LZWS_EXPORT inline void lzws_read_byte(lzws_byte_fast_t* byte_ptr, lzws_byte_t** source_ptr, size_t* source_length_ptr)
{
  *byte_ptr = **source_ptr;
  (*source_ptr)++;
  (*source_length_ptr)--;
}

LZWS_EXPORT inline void
  lzws_write_byte(lzws_byte_fast_t byte, lzws_byte_t** destination_ptr, size_t* destination_length_ptr)
{
  **destination_ptr = byte;
  (*destination_ptr)++;
  (*destination_length_ptr)--;
}

LZWS_EXPORT void
  lzws_fill_array(void* array, size_t size_of_item, size_t length, const void* item_ptr, bool item_bytes_are_identical);

LZWS_EXPORT void* lzws_allocate_array(
  lzws_byte_fast_t size_of_item,
  size_t           length,
  const void*      item_ptr,
  bool             item_is_zero,
  bool             item_bytes_are_identical);

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // LZWS_UTILS_H
