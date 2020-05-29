// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include <string.h>

#include "../../log.h"
#include "../../string.h"

// "0" means default buffer length.
#define BUFFER_LENGTH 0

#define WITHOUT_MAGIC_HEADER false
#define MAX_CODE_BIT_LENGTH LZWS_BIGGEST_MAX_CODE_BIT_LENGTH
#define BLOCK_MODE true
#define MSB false
#define UNALIGNED_BIT_GROUPS false
#define QUIET false

int main()
{
  const char text[]      = "example text";
  size_t     text_length = strlen(text);

  char*  compressed_text;
  size_t compressed_text_length;

  lzws_result_t result = lzws_compress_string(
    (lzws_byte_t*)text, text_length,
    (lzws_byte_t**)&compressed_text, &compressed_text_length, BUFFER_LENGTH,
    WITHOUT_MAGIC_HEADER, MAX_CODE_BIT_LENGTH, BLOCK_MODE, MSB, UNALIGNED_BIT_GROUPS, QUIET);

  if (result != 0) {
    LZWS_LOG_ERROR("string compressor failed");
    return 1;
  }

  // Decompress.

  char*  decompressed_text;
  size_t decompressed_text_length;

  result = lzws_decompress_string(
    (lzws_byte_t*)compressed_text, compressed_text_length,
    (lzws_byte_t**)&decompressed_text, &decompressed_text_length, BUFFER_LENGTH,
    WITHOUT_MAGIC_HEADER, MSB, UNALIGNED_BIT_GROUPS, QUIET);

  free(compressed_text);

  if (result != 0) {
    LZWS_LOG_ERROR("string decompressor failed");
    return 2;
  }

  if (decompressed_text_length != text_length) {
    LZWS_LOG_ERROR("decompressed text length %zu, original length %zu", decompressed_text_length, text_length);
    result = 3;
  }
  else if (strncmp(decompressed_text, text, text_length) != 0) {
    LZWS_LOG_ERROR("decompressed text is not the same as original");
    result = 4;
  }
  else {
    result = 0;
  }

  free(decompressed_text);

  return result;
}
