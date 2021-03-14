// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "buffer.h"

#include "log.h"

static inline lzws_result_t
  create_buffer(lzws_byte_t** buffer_ptr, size_t* buffer_length_ptr, size_t default_buffer_length, bool quiet)
{
  size_t buffer_length = *buffer_length_ptr;
  if (buffer_length == 0) {
    buffer_length = default_buffer_length;
  }

  lzws_byte_t* buffer = malloc(buffer_length);
  if (buffer == NULL) {
    if (!quiet) {
      LZWS_LOG_ERROR("malloc failed, buffer length: %zu", buffer_length);
    }

    return LZWS_BUFFER_ALLOCATE_FAILED;
  }

  *buffer_ptr        = buffer;
  *buffer_length_ptr = buffer_length;

  return 0;
}

LZWS_EXPORT lzws_result_t
  lzws_create_source_buffer_for_compressor(lzws_byte_t** buffer_ptr, size_t* buffer_length_ptr, bool quiet)
{
  return create_buffer(buffer_ptr, buffer_length_ptr, LZWS_DEFAULT_SOURCE_BUFFER_LENGTH_FOR_COMPRESSOR, quiet);
}

LZWS_EXPORT lzws_result_t
  lzws_create_destination_buffer_for_compressor(lzws_byte_t** buffer_ptr, size_t* buffer_length_ptr, bool quiet)
{
  return create_buffer(buffer_ptr, buffer_length_ptr, LZWS_DEFAULT_DESTINATION_BUFFER_LENGTH_FOR_COMPRESSOR, quiet);
}

LZWS_EXPORT lzws_result_t
  lzws_create_source_buffer_for_decompressor(lzws_byte_t** buffer_ptr, size_t* buffer_length_ptr, bool quiet)
{
  return create_buffer(buffer_ptr, buffer_length_ptr, LZWS_DEFAULT_SOURCE_BUFFER_LENGTH_FOR_DECOMPRESSOR, quiet);
}

LZWS_EXPORT lzws_result_t
  lzws_create_destination_buffer_for_decompressor(lzws_byte_t** buffer_ptr, size_t* buffer_length_ptr, bool quiet)
{
  return create_buffer(buffer_ptr, buffer_length_ptr, LZWS_DEFAULT_DESTINATION_BUFFER_LENGTH_FOR_DECOMPRESSOR, quiet);
}

LZWS_EXPORT lzws_result_t lzws_resize_buffer(lzws_byte_t** buffer_ptr, size_t buffer_length, bool quiet)
{
  lzws_byte_t* new_buffer = realloc(*buffer_ptr, buffer_length);
  if (new_buffer == NULL && buffer_length != 0) {
    if (!quiet) {
      LZWS_LOG_ERROR("realloc failed, buffer length: %zu", buffer_length);
    }

    return LZWS_BUFFER_REALLOCATE_FAILED;
  }

  *buffer_ptr = new_buffer;

  return 0;
}
