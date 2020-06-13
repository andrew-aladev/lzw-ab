// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "../code.h"

#include <string.h>

#include "../../log.h"
#include "../../macro.h"
#include "../combination.h"
#include "../string_and_file.h"
#include "code.h"

typedef struct {
  const lzws_code_t* codes;
  const size_t       codes_length;

  const lzws_byte_t* symbols;
  const size_t       symbols_length;
} data_t;

// -- any mode --

// Single code equals symbol.
static const lzws_code_t data0_1[]    = {254};
static const lzws_byte_t symbols0_1[] = {254};

// Each code equals symbol.
static const lzws_code_t data1_2[]    = {250, 4};
static const lzws_byte_t symbols1_2[] = {250, 4};

// Each code equals symbol.
static const lzws_code_t data2_3[]    = {255, 3, 150};
static const lzws_byte_t symbols2_3[] = {255, 3, 150};

static const data_t datas[] = {
  {data0_1, 1, symbols0_1, 1},
  {data1_2, 2, symbols1_2, 2},
  {data2_3, 3, symbols2_3, 3}};
static const size_t datas_length = sizeof(datas) / sizeof(data_t);

// -- block mode --

// Single code equals symbol.
// Clear code shouldn't change symbols.
static const lzws_code_t data3_2[]    = {LZWS_CLEAR_CODE, 100};
static const lzws_byte_t symbols3_1[] = {100};

// Same data with clear code duplicate (UNIX compress compatibility).
static const lzws_code_t data4_3[]    = {LZWS_CLEAR_CODE, LZWS_CLEAR_CODE, 100};
static const lzws_byte_t symbols4_1[] = {100};

// Single code equals symbol.
// Clear code shouldn't change symbols.
static const lzws_code_t data5_2[]    = {5, LZWS_CLEAR_CODE};
static const lzws_byte_t symbols5_1[] = {5};

// Same data with clear code duplicate (UNIX compress compatibility).
static const lzws_code_t data6_3[]    = {5, LZWS_CLEAR_CODE, LZWS_CLEAR_CODE};
static const lzws_byte_t symbols6_1[] = {5};

// First free code used to store 132 + 132 codes.
// Clear code shouldn't change symbols.
static const lzws_code_t data7_3[]    = {132, LZWS_FIRST_FREE_CODE_IN_BLOCK_MODE, LZWS_CLEAR_CODE};
static const lzws_byte_t symbols7_3[] = {132, 132, 132};

// Same data with clear code duplicate (UNIX compress compatibility).
static const lzws_code_t data8_4[]    = {132, LZWS_FIRST_FREE_CODE_IN_BLOCK_MODE, LZWS_CLEAR_CODE, LZWS_CLEAR_CODE};
static const lzws_byte_t symbols8_3[] = {132, 132, 132};

// Each code equals symbol.
// Clear code shouldn't change symbols.
static const lzws_code_t data9_3[]    = {100, LZWS_CLEAR_CODE, 200};
static const lzws_byte_t symbols9_2[] = {100, 200};

// Same data with clear code duplicate (UNIX compress compatibility).
static const lzws_code_t data10_4[]    = {100, LZWS_CLEAR_CODE, LZWS_CLEAR_CODE, 200};
static const lzws_byte_t symbols10_2[] = {100, 200};

// First free code used to store 200 + 200 codes.
// Clear code shouldn't change symbols.
static const lzws_code_t data11_4[]    = {100, LZWS_CLEAR_CODE, 200, LZWS_FIRST_FREE_CODE_IN_BLOCK_MODE};
static const lzws_byte_t symbols11_4[] = {100, 200, 200, 200};

// Same data with clear code duplicate (UNIX compress compatibility).
static const lzws_code_t data12_5[]    = {100, LZWS_CLEAR_CODE, LZWS_CLEAR_CODE, 200, LZWS_FIRST_FREE_CODE_IN_BLOCK_MODE};
static const lzws_byte_t symbols12_4[] = {100, 200, 200, 200};

// Each code equals symbol.
// Clear code shouldn't change symbols.
static const lzws_code_t data13_4[]    = {70, LZWS_CLEAR_CODE, 30, LZWS_CLEAR_CODE};
static const lzws_byte_t symbols13_2[] = {70, 30};

// Same data with clear code duplicate (UNIX compress compatibility).
static const lzws_code_t data14_6[]    = {70, LZWS_CLEAR_CODE, LZWS_CLEAR_CODE, 30, LZWS_CLEAR_CODE, LZWS_CLEAR_CODE};
static const lzws_byte_t symbols14_2[] = {70, 30};

// Second free code from first group used to store 80 + 80 codes.
// First free code from second group used to store 30 + 40 codes.
// Clear code shouldn't change symbols.
static const lzws_code_t data15_8[] = {
  70,
  80,
  LZWS_FIRST_FREE_CODE_IN_BLOCK_MODE + 1,
  LZWS_CLEAR_CODE,
  30,
  40,
  LZWS_FIRST_FREE_CODE_IN_BLOCK_MODE,
  LZWS_CLEAR_CODE};
static const lzws_byte_t symbols15_8[] = {
  70,
  80,
  80,
  80,
  30,
  40,
  30,
  40};

// Same data with clear code duplicate (UNIX compress compatibility).
static const lzws_code_t data16_10[] = {
  70,
  80,
  LZWS_FIRST_FREE_CODE_IN_BLOCK_MODE + 1,
  LZWS_CLEAR_CODE,
  LZWS_CLEAR_CODE,
  30,
  40,
  LZWS_FIRST_FREE_CODE_IN_BLOCK_MODE,
  LZWS_CLEAR_CODE,
  LZWS_CLEAR_CODE};
static const lzws_byte_t symbols16_8[] = {
  70,
  80,
  80,
  80,
  30,
  40,
  30,
  40};

static const data_t datas_for_enabled_block_mode[] = {
  {data3_2, 2, symbols3_1, 1},
  {data4_3, 3, symbols4_1, 1},
  {data5_2, 2, symbols5_1, 1},
  {data6_3, 3, symbols6_1, 1},
  {data7_3, 3, symbols7_3, 3},
  {data8_4, 4, symbols8_3, 3},
  {data9_3, 3, symbols9_2, 2},
  {data10_4, 4, symbols10_2, 2},
  {data11_4, 4, symbols11_4, 4},
  {data12_5, 5, symbols12_4, 4},
  {data13_4, 4, symbols13_2, 2},
  {data14_6, 6, symbols14_2, 2},
  {data15_8, 8, symbols15_8, 8},
  {data16_10, 10, symbols16_8, 8}};
static const size_t datas_for_enabled_block_mode_length = sizeof(datas_for_enabled_block_mode) / sizeof(data_t);

// -- block mode disabled --

// First free code used to store 8 + 8 codes.
static const lzws_code_t data15_2[]    = {8, LZWS_FIRST_FREE_CODE};
static const lzws_byte_t symbols15_3[] = {8, 8, 8};

// Second free code used to store 20 + 20 codes.
static const lzws_code_t data16_3[]    = {15, 20, LZWS_FIRST_FREE_CODE + 1};
static const lzws_byte_t symbols16_4[] = {15, 20, 20, 20};

// First free code used to store 2 + 9 codes.
static const lzws_code_t data17_3[]    = {2, 9, LZWS_FIRST_FREE_CODE};
static const lzws_byte_t symbols17_4[] = {2, 9, 2, 9};

static const data_t datas_for_disabled_block_mode[] = {
  {data15_2, 2, symbols15_3, 3},
  {data16_3, 3, symbols16_4, 4},
  {data17_3, 3, symbols17_4, 4}};
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

  if (result != 0) {
    LZWS_LOG_ERROR("decompressor failed");
    return 2;
  }

  if (destination_length != data_ptr->symbols_length) {
    LZWS_LOG_ERROR("decompressed invalid symbols length %zu, original length %zu", destination_length, data_ptr->symbols_length);
    result = 3;
  }
  else if (strncmp((const char*)destination, (const char*)data_ptr->symbols, destination_length) != 0) {
    LZWS_LOG_ERROR("decompressed symbols are not the same as original");
    result = 4;
  }
  else {
    result = 0;
  }

  free(destination);

  return result;
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
  lzws_result_t result = test_datas(
    compressor_state_ptr, decompressor_state_ptr,
    datas, datas_length, buffer_length);

  if (result != 0) {
    return 1;
  }

  if (compressor_state_ptr->block_mode) {
    result = test_datas(
      compressor_state_ptr, decompressor_state_ptr,
      datas_for_enabled_block_mode, datas_for_enabled_block_mode_length, buffer_length);

    if (result != 0) {
      return 2;
    }
  }
  else {
    result = test_datas(
      compressor_state_ptr, decompressor_state_ptr,
      datas_for_disabled_block_mode, datas_for_disabled_block_mode_length, buffer_length);

    if (result != 0) {
      return 3;
    }
  }

  return 0;
}

lzws_result_t lzws_test_valid_codes()
{
  return lzws_test_compatible_compressor_and_decompressor_combinations(test_all_datas);
}
