// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_COMPRESSOR_HEADER_H)
#define LZWS_COMPRESSOR_HEADER_H

#include "state.h"

// This function is optional.
// Use it to be compatible with original UNIX compress utility.
lzws_result_t lzws_compressor_write_magic_header(lzws_compressor_state_t* state_ptr, lzws_byte_t** destination_ptr,
                                                 size_t* destination_length_ptr);

lzws_result_t lzws_compressor_write_header(lzws_compressor_state_t* state_ptr, lzws_byte_t** destination_ptr,
                                           size_t* destination_length_ptr);

#endif // LZWS_COMPRESSOR_HEADER_H
