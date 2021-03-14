// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_STRING_H)
#define LZWS_STRING_H

#include <stdlib.h>

#include "compressor/common.h"
#include "decompressor/common.h"

#ifdef __cplusplus
extern "C" {
#endif

// Possible results:
enum
{
  LZWS_STRING_ALLOCATE_FAILED = 1,
  LZWS_STRING_VALIDATE_FAILED,
  LZWS_STRING_NOT_ENOUGH_DESTINATION_BUFFER,
  LZWS_STRING_DECOMPRESSOR_CORRUPTED_SOURCE,
  LZWS_STRING_COMPRESSOR_UNEXPECTED_ERROR,
  LZWS_STRING_DECOMPRESSOR_UNEXPECTED_ERROR
};

// Destination buffer length can be equal to 0.
// In this case it will be replaced with default value.

LZWS_EXPORT lzws_result_t lzws_compress_string(
  lzws_byte_t*                     source,
  size_t                           source_length,
  lzws_byte_t**                    destination_ptr,
  size_t*                          destination_length_ptr,
  size_t                           destination_buffer_length,
  const lzws_compressor_options_t* options_ptr);

LZWS_EXPORT lzws_result_t lzws_decompress_string(
  lzws_byte_t*                       source,
  size_t                             source_length,
  lzws_byte_t**                      destination_ptr,
  size_t*                            destination_length_ptr,
  size_t                             destination_buffer_length,
  const lzws_decompressor_options_t* options_ptr);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LZWS_STRING_H
