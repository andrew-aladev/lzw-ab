// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "string_and_file.h"

#include <string.h>

#include "../file.h"
#include "../log.h"
#include "../string.h"

// -- files --

static inline lzws_result_t
  prepare_files(FILE** source_file_ptr, lzws_byte_t* source, size_t source_length, FILE** destination_file_ptr)
{
  FILE* source_file = tmpfile();
  if (source_file == NULL) {
    LZWS_LOG_ERROR("failed to create temp file");
    return LZWS_TEST_STRING_AND_FILE_NATIVE_ERROR;
  }

  size_t written_length = fwrite(source, 1, source_length, source_file);
  if (written_length != source_length) {
    LZWS_LOG_ERROR("failed to write file");
    fclose(source_file);
    return LZWS_TEST_STRING_AND_FILE_NATIVE_ERROR;
  }

  FILE* destination_file = tmpfile();
  if (destination_file == NULL) {
    LZWS_LOG_ERROR("failed to create temp file");
    fclose(source_file);
    return LZWS_TEST_STRING_AND_FILE_NATIVE_ERROR;
  }

  rewind(source_file);

  *source_file_ptr      = source_file;
  *destination_file_ptr = destination_file;

  return 0;
}

static inline lzws_result_t
  read_destination(FILE* destination_file, lzws_byte_t** destination_ptr, size_t destination_length)
{
  lzws_byte_t* destination = malloc(destination_length);
  if (destination == NULL) {
    LZWS_LOG_ERROR("malloc failed, destination length: %zu", destination_length);
    return LZWS_TEST_STRING_AND_FILE_NATIVE_ERROR;
  }

  rewind(destination_file);

  size_t read_length = fread(destination, 1, destination_length, destination_file);
  if (read_length != destination_length || getc(destination_file) != EOF) {
    LZWS_LOG_ERROR("failed to read destination file, file api failed");
    free(destination);
    return LZWS_TEST_STRING_AND_FILE_API_FAILED;
  }

  *destination_ptr = destination;

  return 0;
}

// -- compress --

static inline lzws_result_t test_compress_string(
  lzws_result_t*                   test_result_ptr,
  lzws_byte_t*                     source,
  size_t                           source_length,
  lzws_byte_t**                    destination_ptr,
  size_t*                          destination_length_ptr,
  size_t                           buffer_length,
  const lzws_compressor_options_t* options_ptr)
{
  lzws_byte_t* destination        = NULL;
  size_t       destination_length = 0;

  lzws_result_t test_result =
    lzws_compress_string(source, source_length, &destination, &destination_length, buffer_length, options_ptr);

  // It is fine to receive validation error.
  if (test_result == LZWS_STRING_VALIDATE_FAILED) {
    *test_result_ptr        = test_result;
    *destination_ptr        = NULL;
    *destination_length_ptr = 0;

    return 0;
  }

  if (test_result != 0) {
    LZWS_LOG_ERROR("string api failed");
    return LZWS_TEST_STRING_AND_FILE_API_FAILED;
  }

  *test_result_ptr        = 0;
  *destination_ptr        = destination;
  *destination_length_ptr = destination_length;

  return 0;
}

static inline lzws_result_t test_compress_file_with_destination(
  lzws_result_t*                   test_result_ptr,
  lzws_byte_t*                     source,
  size_t                           source_length,
  lzws_byte_t**                    destination_ptr,
  size_t                           destination_length,
  size_t                           buffer_length,
  const lzws_compressor_options_t* options_ptr)
{
  FILE* source_file;
  FILE* destination_file;

  lzws_result_t result = prepare_files(&source_file, source, source_length, &destination_file);
  if (result != 0) {
    return result;
  }

  lzws_result_t test_result =
    lzws_compress_file(source_file, buffer_length, destination_file, buffer_length, options_ptr);

  fclose(source_file);

  // It is fine to receive validation error.
  if (test_result == LZWS_FILE_VALIDATE_FAILED) {
    *test_result_ptr = test_result;
    *destination_ptr = NULL;

    fclose(destination_file);

    return 0;
  }

  if (test_result != 0) {
    LZWS_LOG_ERROR("file api failed");
    fclose(destination_file);
    return LZWS_TEST_STRING_AND_FILE_API_FAILED;
  }

  lzws_byte_t* destination;
  result = read_destination(destination_file, &destination, destination_length);

  fclose(destination_file);

  if (result != 0) {
    return result;
  }

  *test_result_ptr = 0;
  *destination_ptr = destination;

  return 0;
}

static inline lzws_result_t test_compress_file_without_destination(
  lzws_result_t*                   test_result_ptr,
  lzws_byte_t*                     source,
  size_t                           source_length,
  size_t                           buffer_length,
  const lzws_compressor_options_t* options_ptr)
{
  FILE* source_file;
  FILE* destination_file;

  lzws_result_t result = prepare_files(&source_file, source, source_length, &destination_file);
  if (result != 0) {
    return result;
  }

  lzws_result_t test_result =
    lzws_compress_file(source_file, buffer_length, destination_file, buffer_length, options_ptr);

  fclose(source_file);
  fclose(destination_file);

  // It is fine to receive validation error.
  if (test_result == LZWS_FILE_VALIDATE_FAILED) {
    *test_result_ptr = test_result;

    return 0;
  }

  if (test_result != 0) {
    LZWS_LOG_ERROR("file api failed");
    return LZWS_TEST_STRING_AND_FILE_API_FAILED;
  }

  *test_result_ptr = 0;

  return 0;
}

LZWS_EXPORT lzws_result_t lzws_tests_compress_string_and_file(
  lzws_byte_t*                     source,
  size_t                           source_length,
  lzws_byte_t**                    destination_ptr,
  size_t*                          destination_length_ptr,
  size_t                           buffer_length,
  const lzws_compressor_options_t* options_ptr)
{
  lzws_result_t result, test_result;
  lzws_byte_t*  destination_for_string;
  size_t        destination_length;

  result = test_compress_string(
    &test_result, source, source_length, &destination_for_string, &destination_length, buffer_length, options_ptr);

  if (result != 0) {
    return result;
  }

  if (test_result != 0) {
    LZWS_LOG_ERROR("string compressor failed");

    result = test_compress_file_without_destination(&test_result, source, source_length, buffer_length, options_ptr);
    if (result != 0) {
      return result;
    }

    if (test_result == 0) {
      LZWS_LOG_ERROR("string compressor failed, but file compressor succeed");
      return LZWS_TEST_STRING_AND_FILE_COMPRESSOR_IS_VOLATILE;
    }

    return LZWS_TEST_STRING_AND_FILE_COMPRESSOR_FAILED;
  }

  lzws_byte_t* destination_for_file;

  result = test_compress_file_with_destination(
    &test_result, source, source_length, &destination_for_file, destination_length, buffer_length, options_ptr);

  if (result != 0) {
    free(destination_for_string);
    return result;
  }

  if (test_result != 0) {
    LZWS_LOG_ERROR("string compressor succeed, but file compressor failed");
    free(destination_for_string);
    free(destination_for_file);
    return LZWS_TEST_STRING_AND_FILE_COMPRESSOR_IS_VOLATILE;
  }

  if (strncmp((const char*) destination_for_string, (const char*) destination_for_file, destination_length) != 0) {
    LZWS_LOG_ERROR("string and file compressors returned different results");
    free(destination_for_string);
    free(destination_for_file);
    return LZWS_TEST_STRING_AND_FILE_COMPRESSOR_IS_VOLATILE;
  }

  free(destination_for_file);

  *destination_ptr        = destination_for_string;
  *destination_length_ptr = destination_length;

  return 0;
}

// -- decompress --

static inline lzws_result_t test_decompress_string(
  lzws_result_t*                     test_result_ptr,
  lzws_byte_t*                       source,
  size_t                             source_length,
  lzws_byte_t**                      destination_ptr,
  size_t*                            destination_length_ptr,
  size_t                             buffer_length,
  const lzws_decompressor_options_t* options_ptr)
{
  lzws_byte_t* destination        = NULL;
  size_t       destination_length = 0;

  lzws_result_t test_result =
    lzws_decompress_string(source, source_length, &destination, &destination_length, buffer_length, options_ptr);

  // It is fine to receive validation or corrupted source errors.
  if (test_result == LZWS_STRING_VALIDATE_FAILED || test_result == LZWS_STRING_DECOMPRESSOR_CORRUPTED_SOURCE) {
    *test_result_ptr        = test_result;
    *destination_ptr        = NULL;
    *destination_length_ptr = 0;

    return 0;
  }

  if (test_result != 0) {
    LZWS_LOG_ERROR("string api failed");
    return LZWS_TEST_STRING_AND_FILE_API_FAILED;
  }

  *test_result_ptr        = 0;
  *destination_ptr        = destination;
  *destination_length_ptr = destination_length;

  return 0;
}

static inline lzws_result_t test_decompress_file_with_destination(
  lzws_result_t*                     test_result_ptr,
  lzws_byte_t*                       source,
  size_t                             source_length,
  lzws_byte_t**                      destination_ptr,
  size_t                             destination_length,
  size_t                             buffer_length,
  const lzws_decompressor_options_t* options_ptr)
{
  FILE* source_file;
  FILE* destination_file;

  lzws_result_t result = prepare_files(&source_file, source, source_length, &destination_file);
  if (result != 0) {
    return result;
  }

  lzws_result_t test_result =
    lzws_decompress_file(source_file, buffer_length, destination_file, buffer_length, options_ptr);

  fclose(source_file);

  // It is fine to receive validation or corrupted source errors.
  if (test_result == LZWS_FILE_VALIDATE_FAILED || test_result == LZWS_FILE_DECOMPRESSOR_CORRUPTED_SOURCE) {
    *test_result_ptr = test_result;
    *destination_ptr = NULL;

    fclose(destination_file);

    return 0;
  }

  if (test_result != 0) {
    LZWS_LOG_ERROR("file api failed");
    fclose(destination_file);
    return LZWS_TEST_STRING_AND_FILE_API_FAILED;
  }

  lzws_byte_t* destination;
  result = read_destination(destination_file, &destination, destination_length);

  fclose(destination_file);

  if (result != 0) {
    return result;
  }

  *test_result_ptr = 0;
  *destination_ptr = destination;

  return 0;
}

static inline lzws_result_t test_decompress_file_without_destination(
  lzws_result_t*                     test_result_ptr,
  lzws_byte_t*                       source,
  size_t                             source_length,
  size_t                             buffer_length,
  const lzws_decompressor_options_t* options_ptr)
{
  FILE* source_file;
  FILE* destination_file;

  lzws_result_t result = prepare_files(&source_file, source, source_length, &destination_file);
  if (result != 0) {
    return result;
  }

  lzws_result_t test_result =
    lzws_decompress_file(source_file, buffer_length, destination_file, buffer_length, options_ptr);

  fclose(source_file);
  fclose(destination_file);

  // It is fine to receive validation or corrupted source errors.
  if (test_result == LZWS_FILE_VALIDATE_FAILED || test_result == LZWS_FILE_DECOMPRESSOR_CORRUPTED_SOURCE) {
    *test_result_ptr = test_result;

    return 0;
  }

  if (test_result != 0) {
    LZWS_LOG_ERROR("file api failed");
    return LZWS_TEST_STRING_AND_FILE_API_FAILED;
  }

  *test_result_ptr = 0;

  return 0;
}

LZWS_EXPORT lzws_result_t lzws_tests_decompress_string_and_file(
  lzws_byte_t*                       source,
  size_t                             source_length,
  lzws_byte_t**                      destination_ptr,
  size_t*                            destination_length_ptr,
  size_t                             buffer_length,
  const lzws_decompressor_options_t* options_ptr)
{
  lzws_result_t result, test_result;
  lzws_byte_t*  destination_for_string;
  size_t        destination_length;

  result = test_decompress_string(
    &test_result, source, source_length, &destination_for_string, &destination_length, buffer_length, options_ptr);

  if (result != 0) {
    return result;
  }

  if (test_result != 0) {
    LZWS_LOG_ERROR("string decompressor failed");

    result = test_decompress_file_without_destination(&test_result, source, source_length, buffer_length, options_ptr);
    if (result != 0) {
      return result;
    }

    if (test_result == 0) {
      LZWS_LOG_ERROR("string decompressor failed, but file decompressor succeed");
      return LZWS_TEST_STRING_AND_FILE_DECOMPRESSOR_IS_VOLATILE;
    }

    return LZWS_TEST_STRING_AND_FILE_DECOMPRESSOR_FAILED;
  }

  lzws_byte_t* destination_for_file;

  result = test_decompress_file_with_destination(
    &test_result, source, source_length, &destination_for_file, destination_length, buffer_length, options_ptr);

  if (result != 0) {
    free(destination_for_string);
    return result;
  }

  if (test_result != 0) {
    LZWS_LOG_ERROR("string decompressor succeed, but file decompressor failed");
    free(destination_for_string);
    free(destination_for_file);
    return LZWS_TEST_STRING_AND_FILE_DECOMPRESSOR_IS_VOLATILE;
  }

  if (strncmp((const char*) destination_for_string, (const char*) destination_for_file, destination_length) != 0) {
    LZWS_LOG_ERROR("string and file decompressors returned different results");
    free(destination_for_string);
    free(destination_for_file);
    return LZWS_TEST_STRING_AND_FILE_DECOMPRESSOR_IS_VOLATILE;
  }

  free(destination_for_file);

  *destination_ptr        = destination_for_string;
  *destination_length_ptr = destination_length;

  return 0;
}
