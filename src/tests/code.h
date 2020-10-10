// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_TESTS_CODES_H)
#define LZWS_TESTS_CODES_H

#include "../compressor/state.h"

// Possible results:
enum
{
  LZWS_TEST_CODES_ALLOCATE_FAILED = 1,
  LZWS_TEST_CODES_STATE_INITIALIZE_FAILED,
  LZWS_TEST_CODES_NOT_ENOUGH_DESTINATION_BUFFER,
  LZWS_TEST_CODES_COMPRESSOR_UNEXPECTED_ERROR
};

lzws_result_t lzws_test_compressor_write_codes(
  lzws_compressor_state_t* compressor_state_ptr,
  const lzws_code_t*       codes,
  size_t                   codes_length,
  lzws_byte_t**            destination_ptr,
  size_t*                  destination_length_ptr,
  size_t                   destination_buffer_length);

#endif // LZWS_TESTS_CODES_H
