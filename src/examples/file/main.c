// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include <stdlib.h>
#include <string.h>

#include "../../file.h"
#include "../../log.h"

// "0" means default buffer length.
#define BUFFER_LENGTH 0

int main(void)
{
  const char text[]      = "example text";
  size_t     text_length = strlen(text);

  FILE* text_file = tmpfile();
  if (text_file == NULL) {
    LZWS_LOG_ERROR("failed to create text file");
    return 1;
  }

  size_t written_length = fwrite(text, 1, text_length, text_file);
  if (written_length != text_length) {
    LZWS_LOG_ERROR("failed to write text file");
    fclose(text_file);
    return 2;
  }

  FILE* compressed_text_file = tmpfile();
  if (compressed_text_file == NULL) {
    LZWS_LOG_ERROR("failed to create compressed text file");
    fclose(text_file);
    return 3;
  }

  rewind(text_file);

  lzws_result_t result = lzws_compress_file(text_file, BUFFER_LENGTH, compressed_text_file, BUFFER_LENGTH, NULL);

  fclose(text_file);

  if (result != 0) {
    LZWS_LOG_ERROR("file compressor failed");
    fclose(compressed_text_file);
    return 4;
  }

  // Decompress.

  FILE* decompressed_text_file = tmpfile();
  if (decompressed_text_file == NULL) {
    LZWS_LOG_ERROR("failed to create decompressed text file");
    fclose(compressed_text_file);
    return 5;
  }

  rewind(compressed_text_file);

  result = lzws_decompress_file(compressed_text_file, BUFFER_LENGTH, decompressed_text_file, BUFFER_LENGTH, NULL);

  fclose(compressed_text_file);

  if (result != 0) {
    LZWS_LOG_ERROR("file decompressor failed");
    fclose(decompressed_text_file);
    return 6;
  }

  char* decompressed_text = malloc(text_length);
  if (decompressed_text == NULL) {
    LZWS_LOG_ERROR("malloc failed, text length: %zu", text_length);
    fclose(decompressed_text_file);
    return 7;
  }

  rewind(decompressed_text_file);

  size_t read_length = fread(decompressed_text, 1, text_length, decompressed_text_file);
  if (read_length != text_length || getc(decompressed_text_file) != EOF) {
    LZWS_LOG_ERROR("failed to read decompressed text file");
    free(decompressed_text);
    fclose(decompressed_text_file);
    return 8;
  }

  fclose(decompressed_text_file);

  if (strncmp(decompressed_text, text, text_length) != 0) {
    LZWS_LOG_ERROR("decompressed text is not the same as original");
    free(decompressed_text);
    return 9;
  }

  free(decompressed_text);

  return 0;
}
