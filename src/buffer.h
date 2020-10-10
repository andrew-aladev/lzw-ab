// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_BUFFER_H)
#define LZWS_BUFFER_H

#include <stdlib.h>

#include "common.h"

// Possible results:
enum
{
  LZWS_BUFFER_ALLOCATE_FAILED = 1,
  LZWS_BUFFER_REALLOCATE_FAILED
};

// Compressor dictionary implementations has different performance.
// Buffers shouldn't be a bottleneck.
// We can use bigger buffers for faster implementation.

#if defined(LZWS_COMPRESSOR_DICTIONARY_LINKED_LIST)
#define LZWS_DEFAULT_SOURCE_BUFFER_LENGTH_FOR_COMPRESSOR      (1 << 17) // 128 KB
#define LZWS_DEFAULT_DESTINATION_BUFFER_LENGTH_FOR_COMPRESSOR (1 << 15) // 32 KB
#elif defined(LZWS_COMPRESSOR_DICTIONARY_SPARSE_ARRAY)
#define LZWS_DEFAULT_SOURCE_BUFFER_LENGTH_FOR_COMPRESSOR      (1 << 18) // 256 KB
#define LZWS_DEFAULT_DESTINATION_BUFFER_LENGTH_FOR_COMPRESSOR (1 << 16) // 64 KB
#endif

#define LZWS_DEFAULT_SOURCE_BUFFER_LENGTH_FOR_DECOMPRESSOR      (1 << 16) // 64 KB
#define LZWS_DEFAULT_DESTINATION_BUFFER_LENGTH_FOR_DECOMPRESSOR (1 << 18) // 256 KB

lzws_result_t lzws_create_source_buffer_for_compressor(lzws_byte_t** buffer_ptr, size_t* buffer_length_ptr, bool quiet);
lzws_result_t
  lzws_create_destination_buffer_for_compressor(lzws_byte_t** buffer_ptr, size_t* buffer_length_ptr, bool quiet);
lzws_result_t
  lzws_create_source_buffer_for_decompressor(lzws_byte_t** buffer_ptr, size_t* buffer_length_ptr, bool quiet);
lzws_result_t
  lzws_create_destination_buffer_for_decompressor(lzws_byte_t** buffer_ptr, size_t* buffer_length_ptr, bool quiet);

lzws_result_t lzws_resize_buffer(lzws_byte_t** buffer_ptr, size_t buffer_length, bool quiet);

#endif // LZWS_BUFFER_H
