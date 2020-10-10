// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "string_and_file.h"

#include <string.h>

#include "../file.h"
#include "../log.h"
#include "../string.h"

#define NULL_PATH_NAME "/dev/null"

// -- files --

static inline lzws_result_t open_source_file(FILE** source_file_ptr, lzws_byte_t* source, size_t source_length)
{
  // It is not possible for fmemopen to open empty source file using old glibc.
  // So we have to create 1 byte file and seek to end.

  size_t source_file_length = source_length;
  if (source_length == 0) {
    source_file_length = 1;
  }

  FILE* source_file = fmemopen(source, source_file_length, "r");
  if (source_file == NULL) {
    LZWS_LOG_ERROR("fmemopen for source failed");
    return LZWS_TEST_STRING_AND_FILE_NATIVE_STREAM_FAILED;
  }

  if (source_length == 0 && fseek(source_file, 0, SEEK_END) != 0) {
    LZWS_LOG_ERROR("fseek to the end of file failed");
    return LZWS_TEST_STRING_AND_FILE_NATIVE_STREAM_FAILED;
  }

  *source_file_ptr = source_file;

  return 0;
}

static inline lzws_result_t prepare_files_with_destination(
  FILE**        source_file_ptr,
  lzws_byte_t*  source,
  size_t        source_length,
  FILE**        destination_file_ptr,
  lzws_byte_t** destination_ptr,
  size_t        destination_length)
{
  FILE* source_file;

  lzws_result_t result = open_source_file(&source_file, source, source_length);
  if (result != 0) {
    return result;
  }

  // POSIX group don't want users to use binary mode "b" for fmemopen.
  // So we have to give fmemopen ability to write additional null byte.
  size_t destination_file_length = destination_length + 1;

  lzws_byte_t* destination = malloc(destination_file_length);
  if (destination == NULL) {
    LZWS_LOG_ERROR("malloc failed, buffer length: %zu", destination_file_length);

    fclose(source_file);

    return LZWS_TEST_STRING_AND_FILE_ALLOCATE_FAILED;
  }

  FILE* destination_file = fmemopen(destination, destination_file_length, "w");
  if (destination_file == NULL) {
    LZWS_LOG_ERROR("fmemopen for destination failed");

    fclose(source_file);
    free(destination);

    return LZWS_TEST_STRING_AND_FILE_NATIVE_STREAM_FAILED;
  }

  *source_file_ptr      = source_file;
  *destination_file_ptr = destination_file;
  *destination_ptr      = destination;

  return 0;
}

static inline lzws_result_t prepare_files_without_destination(
  FILE**       source_file_ptr,
  lzws_byte_t* source,
  size_t       source_length,
  FILE**       destination_file_ptr)
{
  FILE* source_file;

  lzws_result_t result = open_source_file(&source_file, source, source_length);
  if (result != 0) {
    return result;
  }

  FILE* destination_file = fopen(NULL_PATH_NAME, "w");
  if (destination_file == NULL) {
    LZWS_LOG_ERROR("fopen for null destination failed");

    fclose(source_file);

    return LZWS_TEST_STRING_AND_FILE_NATIVE_STREAM_FAILED;
  }

  *source_file_ptr      = source_file;
  *destination_file_ptr = destination_file;

  return 0;
}

// -- compress --

static inline lzws_result_t test_compress_string(
  lzws_result_t*   test_result_ptr,
  lzws_byte_t*     source,
  size_t           source_length,
  lzws_byte_t**    destination_ptr,
  size_t*          destination_length_ptr,
  size_t           buffer_length,
  bool             without_magic_header,
  lzws_byte_fast_t max_code_bit_length,
  bool             block_mode,
  bool             msb,
  bool             unaligned_bit_groups)
{
  lzws_byte_t* destination        = NULL;
  size_t       destination_length = 0;

  lzws_result_t test_result = lzws_compress_string(
    source,
    source_length,
    &destination,
    &destination_length,
    buffer_length,
    without_magic_header,
    max_code_bit_length,
    block_mode,
    msb,
    unaligned_bit_groups,
    false);

  if (test_result != 0 && test_result != LZWS_STRING_VALIDATE_FAILED) {
    LZWS_LOG_ERROR("string api failed");
    return LZWS_TEST_STRING_AND_FILE_API_FAILED;
  }

  *test_result_ptr        = test_result;
  *destination_ptr        = destination;
  *destination_length_ptr = destination_length;

  return 0;
}

static inline lzws_result_t test_compress_file_with_destination(
  lzws_result_t*   test_result_ptr,
  lzws_byte_t*     source,
  size_t           source_length,
  lzws_byte_t**    destination_ptr,
  size_t           destination_length,
  size_t           buffer_length,
  bool             without_magic_header,
  lzws_byte_fast_t max_code_bit_length,
  bool             block_mode,
  bool             msb,
  bool             unaligned_bit_groups)
{
  FILE* source_file;
  FILE* destination_file;

  lzws_byte_t* destination;

  lzws_result_t result = prepare_files_with_destination(
    &source_file, source, source_length, &destination_file, &destination, destination_length);

  if (result != 0) {
    return result;
  }

  lzws_result_t test_result = lzws_compress_file(
    source_file,
    buffer_length,
    destination_file,
    buffer_length,
    without_magic_header,
    max_code_bit_length,
    block_mode,
    msb,
    unaligned_bit_groups,
    false);

  fclose(source_file);
  fclose(destination_file);

  if (test_result != 0 && test_result != LZWS_FILE_VALIDATE_FAILED) {
    LZWS_LOG_ERROR("file api failed");
    return LZWS_TEST_STRING_AND_FILE_API_FAILED;
  }

  *test_result_ptr = test_result;
  *destination_ptr = destination;

  return 0;
}

static inline lzws_result_t test_compress_file_without_destination(
  lzws_result_t*   test_result_ptr,
  lzws_byte_t*     source,
  size_t           source_length,
  size_t           buffer_length,
  bool             without_magic_header,
  lzws_byte_fast_t max_code_bit_length,
  bool             block_mode,
  bool             msb,
  bool             unaligned_bit_groups)
{
  FILE* source_file;
  FILE* destination_file;

  lzws_result_t result = prepare_files_without_destination(&source_file, source, source_length, &destination_file);
  if (result != 0) {
    return result;
  }

  lzws_result_t test_result = lzws_compress_file(
    source_file,
    buffer_length,
    destination_file,
    buffer_length,
    without_magic_header,
    max_code_bit_length,
    block_mode,
    msb,
    unaligned_bit_groups,
    false);

  fclose(source_file);
  fclose(destination_file);

  if (test_result != 0 && test_result != LZWS_FILE_VALIDATE_FAILED) {
    LZWS_LOG_ERROR("file api failed");
    return LZWS_TEST_STRING_AND_FILE_API_FAILED;
  }

  *test_result_ptr = test_result;

  return 0;
}

lzws_result_t lzws_tests_compress_string_and_file(
  lzws_byte_t*     source,
  size_t           source_length,
  lzws_byte_t**    destination_ptr,
  size_t*          destination_length_ptr,
  size_t           buffer_length,
  bool             without_magic_header,
  lzws_byte_fast_t max_code_bit_length,
  bool             block_mode,
  bool             msb,
  bool             unaligned_bit_groups)
{
  lzws_result_t result, test_result;

  lzws_byte_t* destination_for_string;
  size_t       destination_length;

  result = test_compress_string(
    &test_result,
    source,
    source_length,
    &destination_for_string,
    &destination_length,
    buffer_length,
    without_magic_header,
    max_code_bit_length,
    block_mode,
    msb,
    unaligned_bit_groups);

  if (result != 0) {
    return result;
  }

  if (test_result != 0) {
    LZWS_LOG_ERROR("string compressor failed");

    result = test_compress_file_without_destination(
      &test_result,
      source,
      source_length,
      buffer_length,
      without_magic_header,
      max_code_bit_length,
      block_mode,
      msb,
      unaligned_bit_groups);

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
    &test_result,
    source,
    source_length,
    &destination_for_file,
    destination_length,
    buffer_length,
    without_magic_header,
    max_code_bit_length,
    block_mode,
    msb,
    unaligned_bit_groups);

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
  lzws_result_t* test_result_ptr,
  lzws_byte_t*   source,
  size_t         source_length,
  lzws_byte_t**  destination_ptr,
  size_t*        destination_length_ptr,
  size_t         buffer_length,
  bool           without_magic_header,
  bool           msb,
  bool           unaligned_bit_groups)
{
  lzws_byte_t* destination        = NULL;
  size_t       destination_length = 0;

  lzws_result_t test_result = lzws_decompress_string(
    source,
    source_length,
    &destination,
    &destination_length,
    buffer_length,
    without_magic_header,
    msb,
    unaligned_bit_groups,
    false);

  if (
    test_result != 0 && test_result != LZWS_STRING_VALIDATE_FAILED &&
    test_result != LZWS_STRING_DECOMPRESSOR_CORRUPTED_SOURCE) {
    LZWS_LOG_ERROR("string api failed");
    return LZWS_TEST_STRING_AND_FILE_API_FAILED;
  }

  *test_result_ptr        = test_result;
  *destination_ptr        = destination;
  *destination_length_ptr = destination_length;

  return 0;
}

static inline lzws_result_t test_decompress_file_with_destination(
  lzws_result_t* test_result_ptr,
  lzws_byte_t*   source,
  size_t         source_length,
  lzws_byte_t**  destination_ptr,
  size_t         destination_length,
  size_t         buffer_length,
  bool           without_magic_header,
  bool           msb,
  bool           unaligned_bit_groups)
{
  FILE* source_file;
  FILE* destination_file;

  lzws_byte_t* destination;

  lzws_result_t result = prepare_files_with_destination(
    &source_file, source, source_length, &destination_file, &destination, destination_length);

  if (result != 0) {
    return result;
  }

  lzws_result_t test_result = lzws_decompress_file(
    source_file,
    buffer_length,
    destination_file,
    buffer_length,
    without_magic_header,
    msb,
    unaligned_bit_groups,
    false);

  fclose(source_file);
  fclose(destination_file);

  if (
    test_result != 0 && test_result != LZWS_FILE_VALIDATE_FAILED &&
    test_result != LZWS_FILE_DECOMPRESSOR_CORRUPTED_SOURCE) {
    LZWS_LOG_ERROR("file api failed");
    return LZWS_TEST_STRING_AND_FILE_API_FAILED;
  }

  *test_result_ptr = test_result;
  *destination_ptr = destination;

  return 0;
}

static inline lzws_result_t test_decompress_file_without_destination(
  lzws_result_t* test_result_ptr,
  lzws_byte_t*   source,
  size_t         source_length,
  size_t         buffer_length,
  bool           without_magic_header,
  bool           msb,
  bool           unaligned_bit_groups)
{
  FILE* source_file;
  FILE* destination_file;

  lzws_result_t result = prepare_files_without_destination(&source_file, source, source_length, &destination_file);
  if (result != 0) {
    return result;
  }

  lzws_result_t test_result = lzws_decompress_file(
    source_file,
    buffer_length,
    destination_file,
    buffer_length,
    without_magic_header,
    msb,
    unaligned_bit_groups,
    false);

  fclose(source_file);
  fclose(destination_file);

  if (
    test_result != 0 && test_result != LZWS_FILE_VALIDATE_FAILED &&
    test_result != LZWS_FILE_DECOMPRESSOR_CORRUPTED_SOURCE) {
    LZWS_LOG_ERROR("file api failed");
    return LZWS_TEST_STRING_AND_FILE_API_FAILED;
  }

  *test_result_ptr = test_result;

  return 0;
}

lzws_result_t lzws_tests_decompress_string_and_file(
  lzws_byte_t*  source,
  size_t        source_length,
  lzws_byte_t** destination_ptr,
  size_t*       destination_length_ptr,
  size_t        buffer_length,
  bool          without_magic_header,
  bool          msb,
  bool          unaligned_bit_groups)
{
  lzws_result_t result, test_result;

  lzws_byte_t* destination_for_string;
  size_t       destination_length;

  result = test_decompress_string(
    &test_result,
    source,
    source_length,
    &destination_for_string,
    &destination_length,
    buffer_length,
    without_magic_header,
    msb,
    unaligned_bit_groups);

  if (result != 0) {
    return result;
  }

  if (test_result != 0) {
    LZWS_LOG_ERROR("string decompressor failed");

    result = test_decompress_file_without_destination(
      &test_result, source, source_length, buffer_length, without_magic_header, msb, unaligned_bit_groups);

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
    &test_result,
    source,
    source_length,
    &destination_for_file,
    destination_length,
    buffer_length,
    without_magic_header,
    msb,
    unaligned_bit_groups);

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
