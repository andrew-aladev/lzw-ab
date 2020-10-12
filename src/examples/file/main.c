// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include <string.h>

#include "../../file.h"
#include "../../log.h"

#define FILE_BUFFER_LENGTH 512

// "0" means default buffer length.
#define BUFFER_LENGTH 0

int main()
{
  const char text[]      = "example text";
  size_t     text_length = strlen(text);

  FILE* text_file = fmemopen((void*) text, text_length, "r");
  if (text_file == NULL) {
    LZWS_LOG_ERROR("fmemopen for text failed");
    return 1;
  }

  char  compressed_text[FILE_BUFFER_LENGTH];
  FILE* compressed_text_file = fmemopen((void*) compressed_text, FILE_BUFFER_LENGTH, "w");
  if (compressed_text_file == NULL) {
    LZWS_LOG_ERROR("fmemopen for compressed text failed");
    fclose(text_file);
    return 2;
  }

  lzws_result_t result = lzws_compress_file(text_file, BUFFER_LENGTH, compressed_text_file, BUFFER_LENGTH, NULL);

  fclose(text_file);

  if (result != 0) {
    LZWS_LOG_ERROR("file compressor failed");
    fclose(compressed_text_file);
    return 3;
  }

  // Compressed text file should be reopened without buffer tail.
  size_t written_length = fwrite("\0", 1, 1, compressed_text_file);

  fclose(compressed_text_file);

  if (written_length != 1) {
    LZWS_LOG_ERROR("fwrite for null terminator failed");
    return 4;
  }

  // Decompress.

  compressed_text_file = fmemopen((void*) compressed_text, strlen(compressed_text), "r");
  if (compressed_text_file == NULL) {
    LZWS_LOG_ERROR("fmemopen for compressed text failed");
    return 5;
  }

  char  decompressed_text[FILE_BUFFER_LENGTH];
  FILE* decompressed_text_file = fmemopen((void*) decompressed_text, FILE_BUFFER_LENGTH, "w");
  if (decompressed_text_file == NULL) {
    LZWS_LOG_ERROR("fmemopen for decompressed text failed");
    fclose(compressed_text_file);
    return 6;
  }

  result = lzws_decompress_file(compressed_text_file, BUFFER_LENGTH, decompressed_text_file, BUFFER_LENGTH, NULL);

  fclose(compressed_text_file);
  fclose(decompressed_text_file);

  if (result != 0) {
    LZWS_LOG_ERROR("file decompressor failed");
    return 7;
  }

  if (strncmp(decompressed_text, text, text_length) != 0) {
    LZWS_LOG_ERROR("decompressed text is not the same as original");
    return 8;
  }

  return 0;
}
