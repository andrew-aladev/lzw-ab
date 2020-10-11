// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_FILE_H)
#define LZWS_FILE_H

#include <stdio.h>

#include "compressor/common.h"
#include "decompressor/common.h"

// Possible results:
enum
{
  LZWS_FILE_ALLOCATE_FAILED = 1,
  LZWS_FILE_VALIDATE_FAILED,
  LZWS_FILE_NOT_ENOUGH_SOURCE_BUFFER,
  LZWS_FILE_NOT_ENOUGH_DESTINATION_BUFFER,
  LZWS_FILE_DECOMPRESSOR_CORRUPTED_SOURCE,
  LZWS_FILE_COMPRESSOR_UNEXPECTED_ERROR,
  LZWS_FILE_DECOMPRESSOR_UNEXPECTED_ERROR,
  LZWS_FILE_READ_FINISHED,
  LZWS_FILE_READ_FAILED,
  LZWS_FILE_WRITE_FAILED
};

// Source buffer length and destination buffer length can be equal to 0.
// In this case it will be replaced with default values.

lzws_result_t lzws_compress_file(
  FILE*                            source_file,
  size_t                           source_buffer_length,
  FILE*                            destination_file,
  size_t                           destination_buffer_length,
  const lzws_compressor_options_t* options_ptr);

lzws_result_t lzws_decompress_file(
  FILE*                              source_file,
  size_t                             source_buffer_length,
  FILE*                              destination_file,
  size_t                             destination_buffer_length,
  const lzws_decompressor_options_t* options_ptr);

#endif // LZWS_FILE_H
