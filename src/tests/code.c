// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "code.h"

#include "../buffer.h"
#include "../compressor/alignment/wrapper.h"
#include "../compressor/code.h"
#include "../compressor/header.h"
#include "../compressor/remainder.h"
#include "../log.h"

static inline lzws_result_t increase_destination_buffer(
  lzws_byte_t** destination_ptr,
  size_t        destination_length,
  size_t*       remaining_destination_buffer_length_ptr,
  size_t        destination_buffer_length)
{
  if (*remaining_destination_buffer_length_ptr == destination_buffer_length) {
    // We want to write more data at once, than buffer has.
    LZWS_LOG_ERROR("not enough destination buffer, length: %zu", destination_buffer_length);
    return LZWS_TEST_CODES_NOT_ENOUGH_DESTINATION_BUFFER;
  }

  lzws_result_t result = lzws_resize_buffer(destination_ptr, destination_length + destination_buffer_length, false);
  if (result != 0) {
    return LZWS_TEST_CODES_ALLOCATE_FAILED;
  }

  *remaining_destination_buffer_length_ptr = destination_buffer_length;

  return 0;
}

#define BUFFERED_COMPRESS(function, ...)                                                                            \
  while (true) {                                                                                                    \
    lzws_byte_t* remaining_destination_buffer             = *destination_ptr + *destination_length_ptr;             \
    size_t       prev_remaining_destination_buffer_length = remaining_destination_buffer_length;                    \
                                                                                                                    \
    result = function(__VA_ARGS__, &remaining_destination_buffer, &remaining_destination_buffer_length);            \
    if (result != 0 && result != LZWS_COMPRESSOR_NEEDS_MORE_DESTINATION) {                                          \
      lzws_compressor_free_state(compressor_state_copy_ptr);                                                        \
      return LZWS_TEST_CODES_COMPRESSOR_UNEXPECTED_ERROR;                                                           \
    }                                                                                                               \
                                                                                                                    \
    *destination_length_ptr += prev_remaining_destination_buffer_length - remaining_destination_buffer_length;      \
                                                                                                                    \
    if (result == LZWS_COMPRESSOR_NEEDS_MORE_DESTINATION) {                                                         \
      result = increase_destination_buffer(                                                                         \
        destination_ptr, *destination_length_ptr, &remaining_destination_buffer_length, destination_buffer_length); \
                                                                                                                    \
      if (result != 0) {                                                                                            \
        lzws_compressor_free_state(compressor_state_copy_ptr);                                                      \
        return result;                                                                                              \
      }                                                                                                             \
                                                                                                                    \
      continue;                                                                                                     \
    }                                                                                                               \
                                                                                                                    \
    break;                                                                                                          \
  }

static inline lzws_result_t compress(
  lzws_compressor_state_t* compressor_state_ptr,
  const lzws_code_t*       codes,
  size_t                   codes_length,
  lzws_byte_t**            destination_ptr,
  size_t*                  destination_length_ptr,
  size_t                   destination_buffer_length)
{
  lzws_result_t            result;
  lzws_compressor_state_t* compressor_state_copy_ptr;

  result = lzws_compressor_get_initial_state(&compressor_state_copy_ptr, &compressor_state_ptr->options);
  if (result != 0) {
    return LZWS_TEST_CODES_STATE_INITIALIZE_FAILED;
  }

  size_t remaining_destination_buffer_length = destination_buffer_length;

  if (!compressor_state_copy_ptr->options.without_magic_header) {
    BUFFERED_COMPRESS(lzws_compressor_write_magic_header, compressor_state_copy_ptr);
  }

  BUFFERED_COMPRESS(lzws_compressor_write_header, compressor_state_copy_ptr);

  lzws_code_t prev_code = LZWS_FIRST_FREE_CODE_IN_BLOCK_MODE;

  for (size_t index = 0; index < codes_length; index++) {
    lzws_code_t code = codes[index];

    if (
      compressor_state_copy_ptr->options.block_mode && !compressor_state_copy_ptr->options.unaligned_bit_groups &&
      prev_code == LZWS_CLEAR_CODE) {
      BUFFERED_COMPRESS(lzws_compressor_write_remainder_before_current_code, compressor_state_copy_ptr);
      BUFFERED_COMPRESS(lzws_compressor_write_alignment_before_current_code, compressor_state_copy_ptr);
    }

    BUFFERED_COMPRESS(lzws_compressor_write_code, compressor_state_copy_ptr, code);

    prev_code = code;
  }

  BUFFERED_COMPRESS(lzws_compressor_flush_remainder, compressor_state_copy_ptr);
  lzws_compressor_free_state(compressor_state_copy_ptr);

  result = lzws_resize_buffer(destination_ptr, *destination_length_ptr, false);
  if (result != 0) {
    return LZWS_TEST_CODES_ALLOCATE_FAILED;
  }

  return 0;
}

LZWS_EXPORT lzws_result_t lzws_test_compressor_write_codes(
  lzws_compressor_state_t* compressor_state_ptr,
  const lzws_code_t*       codes,
  size_t                   codes_length,
  lzws_byte_t**            destination_ptr,
  size_t*                  destination_length_ptr,
  size_t                   destination_buffer_length)
{
  lzws_byte_t* destination_buffer;

  lzws_result_t result =
    lzws_create_destination_buffer_for_compressor(&destination_buffer, &destination_buffer_length, false);

  if (result != 0) {
    return LZWS_TEST_CODES_ALLOCATE_FAILED;
  }

  *destination_ptr        = destination_buffer;
  *destination_length_ptr = 0;

  result = compress(
    compressor_state_ptr, codes, codes_length, destination_ptr, destination_length_ptr, destination_buffer_length);

  if (result != 0) {
    free(*destination_ptr);
    return result;
  }

  return 0;
}
