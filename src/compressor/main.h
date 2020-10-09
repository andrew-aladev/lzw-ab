// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_COMPRESSOR_MAIN_H)
#define LZWS_COMPRESSOR_MAIN_H

#include "state.h"

lzws_result_t lzws_compress(lzws_compressor_state_t* state_ptr, lzws_byte_t** source_ptr, size_t* source_length_ptr,
                            lzws_byte_t** destination_ptr, size_t* destination_length_ptr);

// Use this function when you have no more source (received EOF for example).
lzws_result_t lzws_compressor_finish(lzws_compressor_state_t* state_ptr, lzws_byte_t** destination_ptr,
                                     size_t* destination_length_ptr);

#endif // LZWS_COMPRESSOR_MAIN_H
