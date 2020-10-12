// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "header.h"

#include "../../compressor/header.h"
#include "../../decompressor/header.h"
#include "../../log.h"
#include "../../macro.h"
#include "../combination.h"

#define MAGIC_HEADER_SIZE 2
static const lzws_byte_t magic_headers[][MAGIC_HEADER_SIZE] = {
  {LZWS_FIRST_MAGIC_HEADER_BYTE + 1, LZWS_SECOND_MAGIC_HEADER_BYTE}, // First byte is invalid.
  {LZWS_FIRST_MAGIC_HEADER_BYTE, LZWS_SECOND_MAGIC_HEADER_BYTE + 1}  // Second byte is invalid.
};
#define MAGIC_HEADER_LENGTH sizeof(magic_headers) / MAGIC_HEADER_SIZE

#define HEADER_SIZE 1
static const lzws_byte_t headers[][HEADER_SIZE] = {{(LZWS_LOWEST_MAX_CODE_BIT_LENGTH - 1) | LZWS_BLOCK_MODE}};
#define HEADER_LENGTH sizeof(headers) / HEADER_SIZE

static inline lzws_result_t test_invalid_header(
  lzws_compressor_state_t*   compressor_state_ptr,
  lzws_decompressor_state_t* decompressor_state_ptr,
  size_t                     LZWS_UNUSED(buffer_length),
  va_list                    LZWS_UNUSED(args))
{
  lzws_result_t result;
  size_t        index;
  lzws_byte_t*  header;
  size_t        header_size;

  // Magic header.

  if (!compressor_state_ptr->options.without_magic_header) {
    header_size = MAGIC_HEADER_SIZE - 1;
    result      = lzws_compressor_write_magic_header(compressor_state_ptr, NULL, &header_size);
    if (result != LZWS_COMPRESSOR_NEEDS_MORE_DESTINATION) {
      LZWS_LOG_ERROR("compressor write magic header should require more destination");
      return 1;
    }
  }

  if (!decompressor_state_ptr->options.without_magic_header) {
    header_size = MAGIC_HEADER_SIZE - 1;
    result      = lzws_decompressor_read_magic_header(decompressor_state_ptr, NULL, &header_size);
    if (result != LZWS_DECOMPRESSOR_NEEDS_MORE_SOURCE) {
      LZWS_LOG_ERROR("decompressor read magic header should require more source");
      return 2;
    }

    for (index = 0; index < MAGIC_HEADER_LENGTH; index++) {
      header      = (lzws_byte_t*) magic_headers[index];
      header_size = MAGIC_HEADER_SIZE;

      result = lzws_decompressor_read_magic_header(decompressor_state_ptr, &header, &header_size);
      if (result != LZWS_DECOMPRESSOR_INVALID_MAGIC_HEADER) {
        LZWS_LOG_ERROR("decompressor read magic header should fail with invalid magic header");
        return 3;
      }
    }
  }

  // Header.

  header_size = HEADER_SIZE - 1;
  result      = lzws_compressor_write_header(compressor_state_ptr, NULL, &header_size);
  if (result != LZWS_COMPRESSOR_NEEDS_MORE_DESTINATION) {
    LZWS_LOG_ERROR("compressor write header should require more destination");
    return 4;
  }

  header_size = HEADER_SIZE - 1;
  result      = lzws_decompressor_read_header(decompressor_state_ptr, NULL, &header_size);
  if (result != LZWS_DECOMPRESSOR_NEEDS_MORE_SOURCE) {
    LZWS_LOG_ERROR("decompressor read header should require more source");
    return 5;
  }

  for (index = 0; index < HEADER_LENGTH; index++) {
    header      = (lzws_byte_t*) headers[index];
    header_size = HEADER_SIZE;

    result = lzws_decompressor_read_header(decompressor_state_ptr, &header, &header_size);
    if (result != LZWS_DECOMPRESSOR_INVALID_MAX_CODE_BIT_LENGTH) {
      LZWS_LOG_ERROR("decompressor read header should fail with invalid max code bit length");
      return 6;
    }
  }

  return 0;
}

lzws_result_t lzws_test_invalid_header()
{
  return lzws_test_compatible_compressor_and_decompressor_combinations(test_invalid_header);
}
