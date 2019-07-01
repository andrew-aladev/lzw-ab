// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "../log.h"

#include "combination.h"

// "0" means default buffer length.
// "2" bytes is the minimal buffer length that should be enough for both compressor and decompressor.
// "3" bytes should be enough for writing and reading header.
static const size_t buffer_lengths[] = {0, 2, 3, 512};
#define BUFFER_LENGTH_SIZE sizeof(buffer_lengths) / sizeof(size_t)

static const bool without_magic_headers[] = {true, false};
#define WITHOUT_MAGIC_HEADERS_LENGTH sizeof(without_magic_headers) / sizeof(bool)

static const bool block_modes[] = {true, false};
#define BLOCK_MODE_LENGTH sizeof(block_modes) / sizeof(bool)

static const bool msbs[] = {true, false};
#define MSB_LENGTH sizeof(msbs) / sizeof(bool)

static const bool unaligned_bit_groups_values[] = {true, false};
#define UNALIGNED_BIT_GROUPS_LENGTH sizeof(unaligned_bit_groups_values) / sizeof(bool)

// -- compressor --

static inline lzws_result_t test_compressor(
  lzws_test_compressor_t function, va_list args, size_t buffer_length,
  bool without_magic_header, uint_fast8_t max_code_bit_length, bool block_mode, bool msb, bool unaligned_bit_groups)
{
  lzws_compressor_state_t* state_ptr;

  lzws_result_t result = lzws_compressor_get_initial_state(&state_ptr, max_code_bit_length, block_mode, msb, unaligned_bit_groups, false);
  if (result != 0) {
    LZWS_LOG_ERROR("failed to get initial state for compressor");
    return result;
  }

  // We need to copy arguments for each function call.
  va_list args_copy;
  va_copy(args_copy, args);

  result = function(state_ptr, buffer_length, without_magic_header, args_copy);

  lzws_compressor_free_state(state_ptr);

  return result;
}

static inline lzws_result_t process_compressor_combinations(lzws_test_compressor_t function, va_list args)
{
  for (size_t index = 0; index < BUFFER_LENGTH_SIZE; index++) {
    size_t buffer_length = buffer_lengths[index];

    for (size_t jndex = 0; jndex < WITHOUT_MAGIC_HEADERS_LENGTH; jndex++) {
      bool without_magic_header = without_magic_headers[jndex];

      for (uint_fast8_t max_code_bit_length = LZWS_LOWEST_MAX_CODE_BIT_LENGTH; max_code_bit_length <= LZWS_BIGGEST_MAX_CODE_BIT_LENGTH; max_code_bit_length++) {
        for (size_t kndex = 0; kndex < BLOCK_MODE_LENGTH; kndex++) {
          bool block_mode = block_modes[kndex];

          for (size_t lndex = 0; lndex < MSB_LENGTH; lndex++) {
            bool msb = msbs[lndex];

            for (size_t mndex = 0; mndex < UNALIGNED_BIT_GROUPS_LENGTH; mndex++) {
              bool unaligned_bit_groups = unaligned_bit_groups_values[mndex];

              lzws_result_t result = test_compressor(
                function, args, buffer_length,
                without_magic_header, max_code_bit_length, block_mode, msb, unaligned_bit_groups);
              if (result != 0) {
                return result;
              }
            }
          }
        }
      }
    }
  }

  return 0;
}

lzws_result_t lzws_test_compressor_combinations(lzws_test_compressor_t function, ...)
{
  va_list args;
  va_start(args, function);

  lzws_result_t result = process_compressor_combinations(function, args);

  va_end(args);

  return result;
}

// -- decompressor --

static inline lzws_result_t test_decompressor(
  lzws_test_decompressor_t function, va_list args, size_t buffer_length,
  bool without_magic_header, bool msb, bool unaligned_bit_groups)
{
  lzws_decompressor_state_t* state_ptr;

  lzws_result_t result = lzws_decompressor_get_initial_state(&state_ptr, msb, unaligned_bit_groups, false);
  if (result != 0) {
    LZWS_LOG_ERROR("failed to get initial state for decompressor");
    return result;
  }

  // We need to copy arguments for each function call.
  va_list args_copy;
  va_copy(args_copy, args);

  result = function(state_ptr, buffer_length, without_magic_header, args_copy);

  lzws_decompressor_free_state(state_ptr);

  return result;
}

lzws_result_t process_decompressor_combinations(lzws_test_decompressor_t function, va_list args)
{
  for (size_t index = 0; index < BUFFER_LENGTH_SIZE; index++) {
    size_t buffer_length = buffer_lengths[index];

    for (size_t jndex = 0; jndex < WITHOUT_MAGIC_HEADERS_LENGTH; jndex++) {
      bool without_magic_header = without_magic_headers[jndex];

      for (size_t kndex = 0; kndex < MSB_LENGTH; kndex++) {
        bool msb = msbs[kndex];

        for (size_t lndex = 0; lndex < UNALIGNED_BIT_GROUPS_LENGTH; lndex++) {
          bool unaligned_bit_groups = unaligned_bit_groups_values[lndex];

          lzws_result_t result = test_decompressor(
            function, args, buffer_length,
            without_magic_header, msb, unaligned_bit_groups);
          if (result != 0) {
            return result;
          }
        }
      }
    }
  }

  return 0;
}

lzws_result_t lzws_test_decompressor_combinations(lzws_test_decompressor_t function, ...)
{
  va_list args;
  va_start(args, function);

  lzws_result_t result = process_decompressor_combinations(function, args);

  va_end(args);

  return result;
}

// -- compressor and decompressor --

static inline lzws_result_t test_combination_with_compressor_and_decompressor(
  lzws_compressor_state_t* compressor_state_ptr, size_t compressor_buffer_length,
  bool    compressor_without_magic_header,
  va_list args)
{
  lzws_decompressor_state_t*              decompressor_state_ptr            = va_arg(args, lzws_decompressor_state_t*);
  size_t                                  decompressor_buffer_length        = va_arg(args, size_t);
  bool                                    decompressor_without_magic_header = va_arg(args, int);
  lzws_test_compressor_and_decompressor_t function                          = va_arg(args, lzws_test_compressor_and_decompressor_t);
  va_list*                                function_args                     = va_arg(args, va_list*);

  // We need to ignore difference between buffer lengths and without magic header.
  if (compressor_buffer_length != decompressor_buffer_length || compressor_without_magic_header != decompressor_without_magic_header) {
    return 0;
  }

  // We need to copy arguments for each function call.
  va_list function_args_copy;
  va_copy(function_args_copy, *function_args);

  return function(
    compressor_state_ptr, decompressor_state_ptr, compressor_buffer_length,
    compressor_without_magic_header,
    function_args_copy);
}

static inline lzws_result_t test_compressor_combinations_with_decompressor(
  lzws_decompressor_state_t* decompressor_state_ptr, size_t decompressor_buffer_length,
  bool    decompressor_without_magic_header,
  va_list args)
{
  lzws_test_compressor_and_decompressor_t function      = va_arg(args, lzws_test_compressor_and_decompressor_t);
  va_list*                                function_args = va_arg(args, va_list*);

  return lzws_test_compressor_combinations(
    test_combination_with_compressor_and_decompressor,
    decompressor_state_ptr, decompressor_buffer_length,
    decompressor_without_magic_header,
    function, function_args);
}

lzws_result_t lzws_test_compressor_and_decompressor_combinations(lzws_test_compressor_and_decompressor_t function, ...)
{
  va_list function_args;
  va_start(function_args, function);

  lzws_result_t result = lzws_test_decompressor_combinations(test_compressor_combinations_with_decompressor, function, &function_args);

  va_end(function_args);

  return result;
}

// -- compatible compressor and decompressor --

static inline lzws_result_t test_compatible_compressor_and_decompressor_combination(
  lzws_compressor_state_t* compressor_state_ptr, lzws_decompressor_state_t* decompressor_state_ptr, size_t buffer_length,
  bool    without_magic_header,
  va_list args)
{
  if (compressor_state_ptr->msb != decompressor_state_ptr->msb ||
      compressor_state_ptr->unaligned_bit_groups != decompressor_state_ptr->unaligned_bit_groups) {
    return 0;
  }

  lzws_test_compressor_and_decompressor_t function      = va_arg(args, lzws_test_compressor_and_decompressor_t);
  va_list*                                function_args = va_arg(args, va_list*);

  // We need to copy arguments for each function call.
  va_list function_args_copy;
  va_copy(function_args_copy, *function_args);

  return function(
    compressor_state_ptr, decompressor_state_ptr, buffer_length,
    without_magic_header,
    function_args_copy);
}

lzws_result_t lzws_test_compatible_compressor_and_decompressor_combinations(lzws_test_compressor_and_decompressor_t function, ...)
{
  va_list function_args;
  va_start(function_args, function);

  lzws_result_t result = lzws_test_compressor_and_decompressor_combinations(test_compatible_compressor_and_decompressor_combination, function, &function_args);

  va_end(function_args);

  return result;
}
