// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "data.h"

#include <string.h>

#include "../../common.h"
#include "../../log.h"
#include "../../macro.h"
#include "../../utils.h"
#include "../combination.h"
#include "../random_string.h"
#include "../string_and_file.h"

static const char* datas[] = {
  "",
  "hello world",
  "tobeornottobeortobeornot"};
#define DATA_LENGTH 3

#define RANDOM_STRING_LENGTH (1 << 14) // 16 KB

static inline lzws_result_t test_data(
  lzws_compressor_state_t* compressor_state_ptr, lzws_decompressor_state_t* decompressor_state_ptr,
  const char* data, size_t buffer_length)
{
  size_t data_length = strlen(data);

  lzws_byte_t* compressed_data;
  size_t       compressed_data_length;

  lzws_result_t result = lzws_tests_compress_string_and_file(
    (lzws_byte_t*)data, data_length,
    &compressed_data, &compressed_data_length,
    buffer_length,
    compressor_state_ptr->without_magic_header,
    compressor_state_ptr->max_code_bit_length,
    compressor_state_ptr->block_mode,
    compressor_state_ptr->msb,
    compressor_state_ptr->unaligned_bit_groups);

  if (result != 0) {
    return 1;
  }

  char*  result_data;
  size_t result_data_length;

  result = lzws_tests_decompress_string_and_file(
    compressed_data, compressed_data_length,
    (lzws_byte_t**)&result_data, &result_data_length,
    buffer_length,
    decompressor_state_ptr->without_magic_header,
    decompressor_state_ptr->msb,
    decompressor_state_ptr->unaligned_bit_groups);

  free(compressed_data);

  if (result != 0) {
    return 2;
  }

  if (result_data_length != data_length) {
    LZWS_LOG_ERROR("decompressed invalid data length %zu, original length %zu", result_data_length, data_length);
    result = 3;
  }
  else if (strncmp(result_data, data, data_length) != 0) {
    LZWS_LOG_ERROR("decompressed data is not the same as original");
    result = 4;
  }
  else {
    result = 0;
  }

  free(result_data);

  return result;
}

static inline lzws_result_t test_random_string(
  lzws_compressor_state_t* compressor_state_ptr, lzws_decompressor_state_t* decompressor_state_ptr,
  size_t buffer_length)
{
  char* random_string = malloc(RANDOM_STRING_LENGTH);
  if (random_string == NULL) {
    LZWS_LOG_ERROR("malloc failed, string size: %u", RANDOM_STRING_LENGTH);
    return 1;
  }

  lzws_tests_set_random_string(random_string, RANDOM_STRING_LENGTH);
  lzws_result_t result = test_data(
    compressor_state_ptr, decompressor_state_ptr,
    random_string, buffer_length);

  free(random_string);

  if (result != 0) {
    return 2;
  }

  return 0;
}

// We need to test a special case: eof after completed bits group.
// Algorithm creates a next code (with another bit length), saves it into the dictionary, but doesn't write it.
// Than eof appears and algorithm have to write alignment and this code.

// We can use just string filled with single symbol.
// "aa" - first code, "aaa" - second code, ... "a...a" (n + 1) - "n" code.
// Last symbol of first code is the first symbol of second code.
// Length of "n" codes string: (2 - 1) + (3 - 1) + ... + (n - 1) + n + 1 = n * (n + 1) / 2 + 1.
// "n" should be equal to first free code with another bit length.

static inline lzws_result_t test_eof_after_completed_bits_group(
  lzws_compressor_state_t* compressor_state_ptr, lzws_decompressor_state_t* decompressor_state_ptr,
  size_t buffer_length)
{
  lzws_code_fast_t first_code    = lzws_get_first_free_code(compressor_state_ptr->block_mode);
  lzws_code_fast_t target_number = (1 << LZWS_LOWEST_MAX_CODE_BIT_LENGTH) - first_code + 1;
  size_t           string_length = target_number * (target_number + 1) / 2 + 2; // + 1 byte for \0.

  char  symbol = 'a';
  char* string = lzws_allocate_array(1, string_length, (void*)&symbol, false, true);
  if (string == NULL) {
    LZWS_LOG_ERROR("allocate array failed, string size: %zu", string_length);
    return 1;
  }

  string[string_length - 1] = '\0';

  lzws_result_t result = test_data(
    compressor_state_ptr, decompressor_state_ptr,
    string, buffer_length);

  free(string);

  if (result != 0) {
    return 2;
  }

  return 0;
}

static inline lzws_result_t test_datas(
  lzws_compressor_state_t* compressor_state_ptr, lzws_decompressor_state_t* decompressor_state_ptr,
  size_t buffer_length, va_list LZWS_UNUSED(args))
{
  lzws_result_t result;

  for (size_t index = 0; index < DATA_LENGTH; index++) {
    result = test_data(
      compressor_state_ptr, decompressor_state_ptr,
      datas[index], buffer_length);

    if (result != 0) {
      return 1;
    }
  }

  result = test_random_string(
    compressor_state_ptr, decompressor_state_ptr,
    buffer_length);

  if (result != 0) {
    return 2;
  }

  result = test_eof_after_completed_bits_group(
    compressor_state_ptr, decompressor_state_ptr,
    buffer_length);

  if (result != 0) {
    return 3;
  }

  return 0;
}

lzws_result_t lzws_test_valid_datas()
{
  return lzws_test_compatible_compressor_and_decompressor_combinations(test_datas);
}
