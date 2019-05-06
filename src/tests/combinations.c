// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "../log.h"

#include "combinations.h"

static const bool block_modes[] = {true, false};
#define BLOCK_MODE_LENGTH sizeof(block_modes) / sizeof(bool)

static const bool msbs[] = {true, false};
#define MSB_LENGTH sizeof(msbs) / sizeof(bool)

static const bool unaligned_bit_groups_values[] = {true, false};
#define UNALIGNED_BIT_GROUPS_LENGTH sizeof(unaligned_bit_groups_values) / sizeof(bool)

// "0" means automatic buffer length.
// "2" is the minimal buffer length that should be enough for both compressor and decompressor.
static const size_t buffer_lengths[] = {0, 2, 512};
#define BUFFER_LENGTH_SIZE sizeof(buffer_lengths) / sizeof(size_t)

// -- compressor --

static inline lzws_result_t test_compressor(
  lzws_test_compressor_t function, va_list args,
  uint_fast8_t max_code_bit_length, bool block_mode, bool msb, bool unaligned_bit_groups,
  size_t buffer_length)
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

  result = function(state_ptr, buffer_length, args_copy);

  lzws_compressor_free_state(state_ptr);

  return result;
}

static inline lzws_result_t process_compressor_combinations(lzws_test_compressor_t function, va_list args)
{
  for (uint_fast8_t max_code_bit_length = LZWS_LOWEST_MAX_CODE_BIT_LENGTH; max_code_bit_length <= LZWS_BIGGEST_MAX_CODE_BIT_LENGTH; max_code_bit_length++) {
    for (size_t index = 0; index < BLOCK_MODE_LENGTH; index++) {
      bool block_mode = block_modes[index];

      for (size_t jndex = 0; jndex < MSB_LENGTH; jndex++) {
        bool msb = msbs[jndex];

        for (size_t kndex = 0; kndex < UNALIGNED_BIT_GROUPS_LENGTH; kndex++) {
          bool unaligned_bit_groups = unaligned_bit_groups_values[kndex];

          for (size_t mndex = 0; mndex < BUFFER_LENGTH_SIZE; mndex++) {
            size_t buffer_length = buffer_lengths[mndex];

            lzws_result_t result = test_compressor(
              function, args,
              max_code_bit_length, block_mode, msb, unaligned_bit_groups,
              buffer_length);
            if (result != 0) {
              return result;
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
  lzws_test_decompressor_t function, va_list args,
  bool msb, bool unaligned_bit_groups,
  size_t buffer_length)
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

  result = function(state_ptr, buffer_length, args_copy);

  lzws_decompressor_free_state(state_ptr);

  return result;
}

lzws_result_t process_decompressor_combinations(lzws_test_decompressor_t function, va_list args)
{
  for (size_t index = 0; index < MSB_LENGTH; index++) {
    bool msb = msbs[index];

    for (size_t jndex = 0; jndex < UNALIGNED_BIT_GROUPS_LENGTH; jndex++) {
      bool unaligned_bit_groups = unaligned_bit_groups_values[jndex];

      for (size_t kndex = 0; kndex < BUFFER_LENGTH_SIZE; kndex++) {
        size_t buffer_length = buffer_lengths[kndex];

        lzws_result_t result = test_decompressor(
          function, args,
          msb, unaligned_bit_groups,
          buffer_length);
        if (result != 0) {
          return result;
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

static inline lzws_result_t test_combination_with_compressor_and_decompressor(lzws_compressor_state_t* compressor_state_ptr, size_t compressor_buffer_length, va_list args)
{
  lzws_decompressor_state_t*              decompressor_state_ptr     = va_arg(args, lzws_decompressor_state_t*);
  size_t                                  decompressor_buffer_length = va_arg(args, size_t);
  lzws_test_compressor_and_decompressor_t function                   = va_arg(args, lzws_test_compressor_and_decompressor_t);
  va_list*                                function_args              = va_arg(args, va_list*);

  // We need to ignore difference between buffer lengths.
  if (compressor_buffer_length != decompressor_buffer_length) {
    return 0;
  }

  // We need to copy arguments for each function call.
  va_list function_args_copy;
  va_copy(function_args_copy, *function_args);

  return function(compressor_state_ptr, decompressor_state_ptr, compressor_buffer_length, function_args_copy);
}

static inline lzws_result_t test_compressor_combinations_with_decompressor(lzws_decompressor_state_t* decompressor_state_ptr, size_t decompressor_buffer_length, va_list args)
{
  lzws_test_compressor_and_decompressor_t function      = va_arg(args, lzws_test_compressor_and_decompressor_t);
  va_list*                                function_args = va_arg(args, va_list*);

  return lzws_test_compressor_combinations(
    test_combination_with_compressor_and_decompressor,
    decompressor_state_ptr, decompressor_buffer_length, function, function_args);
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
  lzws_compressor_state_t* compressor_state_ptr, lzws_decompressor_state_t* decompressor_state_ptr, size_t buffer_length, va_list args)
{
  if (
    compressor_state_ptr->msb != decompressor_state_ptr->msb ||
    compressor_state_ptr->unaligned_bit_groups != decompressor_state_ptr->unaligned_bit_groups) {
    return 0;
  }

  lzws_test_compressor_and_decompressor_t function      = va_arg(args, lzws_test_compressor_and_decompressor_t);
  va_list*                                function_args = va_arg(args, va_list*);

  // We need to copy arguments for each function call.
  va_list function_args_copy;
  va_copy(function_args_copy, *function_args);

  return function(compressor_state_ptr, decompressor_state_ptr, buffer_length, function_args_copy);
}

lzws_result_t lzws_test_compatible_compressor_and_decompressor_combinations(lzws_test_compressor_and_decompressor_t function, ...)
{
  va_list function_args;
  va_start(function_args, function);

  lzws_result_t result = lzws_test_compressor_and_decompressor_combinations(test_compatible_compressor_and_decompressor_combination, function, &function_args);

  va_end(function_args);

  return result;
}
