// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "../code.h"

#include "../../log.h"
#include "../../macro.h"
#include "../combination.h"
#include "../string_and_file.h"
#include "code.h"

typedef struct {
  const lzws_code_t* codes;
  const size_t       codes_length;
} data_t;

// -- block mode --

// First free code should be a char code.
static const lzws_code_t data0_1[] = {LZWS_FIRST_FREE_CODE_IN_BLOCK_MODE};

// Last code is greater than next code (equals to first free code).
static const lzws_code_t data1_2[] = {1, LZWS_FIRST_FREE_CODE_IN_BLOCK_MODE + 1};

// Code after clear code should be a char code.
static const lzws_code_t data2_3[] = {1, LZWS_CLEAR_CODE, LZWS_FIRST_FREE_CODE_IN_BLOCK_MODE};

// Last code is greater than next code (equals to first free code).
static const lzws_code_t data3_4[] = {1, LZWS_CLEAR_CODE, 1, LZWS_FIRST_FREE_CODE_IN_BLOCK_MODE + 1};

static const data_t datas_for_enabled_block_mode[] = {
  {data0_1, 1},
  {data1_2, 2},
  {data2_3, 3},
  {data3_4, 4}};
static const size_t datas_for_enabled_block_mode_length = sizeof(datas_for_enabled_block_mode) / sizeof(data_t);

// -- block mode disabled --

// First free code should be a char code.
static const lzws_code_t data4_1[] = {LZWS_FIRST_FREE_CODE};

// Last code is greater than next code (equals to first free code).
static const lzws_code_t data5_2[] = {1, LZWS_FIRST_FREE_CODE + 1};

static const data_t datas_for_disabled_block_mode[] = {
  {data4_1, 1},
  {data5_2, 2}};
static const size_t datas_for_disabled_block_mode_length = sizeof(datas_for_disabled_block_mode) / sizeof(data_t);

// -- test --

static inline lzws_result_t test_data(
  lzws_compressor_state_t* compressor_state_ptr, lzws_decompressor_state_t* decompressor_state_ptr,
  const data_t* data_ptr, size_t buffer_length)
{
  lzws_byte_t* source;
  size_t       source_length;

  lzws_result_t result = lzws_test_compressor_write_codes(
    compressor_state_ptr,
    data_ptr->codes, data_ptr->codes_length,
    &source, &source_length, buffer_length);

  if (result != 0) {
    LZWS_LOG_ERROR("compressor failed to write codes");
    return 1;
  }

  lzws_compressor_reset_last_used_data(compressor_state_ptr);

  lzws_byte_t* destination;
  size_t       destination_length;

  result = lzws_tests_decompress_string_and_file(
    source, source_length,
    &destination, &destination_length,
    buffer_length,
    decompressor_state_ptr->without_magic_header,
    decompressor_state_ptr->msb,
    decompressor_state_ptr->unaligned_bit_groups);

  free(source);

  if (result == 0) {
    free(destination);
    LZWS_LOG_ERROR("decompressor succeeded instead of fail");
    return 2;
  }
  if (result != LZWS_TEST_STRING_AND_FILE_DECOMPRESSOR_FAILED) {
    LZWS_LOG_ERROR("decompressor failed with wrong result");
    return 3;
  }

  return 0;
}

static inline lzws_result_t test_datas(
  lzws_compressor_state_t* compressor_state_ptr, lzws_decompressor_state_t* decompressor_state_ptr,
  const data_t* datas, size_t datas_length, size_t buffer_length)
{
  lzws_result_t result;

  for (size_t index = 0; index < datas_length; index++) {
    result = test_data(
      compressor_state_ptr, decompressor_state_ptr,
      &datas[index], buffer_length);

    if (result != 0) {
      return result;
    }
  }

  return 0;
}

static inline lzws_result_t test_all_datas(
  lzws_compressor_state_t* compressor_state_ptr, lzws_decompressor_state_t* decompressor_state_ptr,
  size_t buffer_length, va_list LZWS_UNUSED(args))
{
  lzws_result_t result;

  if (compressor_state_ptr->block_mode) {
    // Codes test won't provide alignment bits.
    if (compressor_state_ptr->unaligned_bit_groups) {
      result = test_datas(
        compressor_state_ptr, decompressor_state_ptr,
        datas_for_enabled_block_mode, datas_for_enabled_block_mode_length, buffer_length);

      if (result != 0) {
        return 1;
      }
    }
  }
  else {
    result = test_datas(
      compressor_state_ptr, decompressor_state_ptr,
      datas_for_disabled_block_mode, datas_for_disabled_block_mode_length, buffer_length);

    if (result != 0) {
      return 2;
    }
  }

  return 0;
}

lzws_result_t lzws_test_invalid_codes()
{
  return lzws_test_compatible_compressor_and_decompressor_combinations(test_all_datas);
}
