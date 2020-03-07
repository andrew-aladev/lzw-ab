// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_COMPRESSOR_UTILS_H)
#define LZWS_COMPRESSOR_UTILS_H

#include "state.h"

void lzws_compressor_read_byte(lzws_compressor_state_t* state_ptr, lzws_symbol_fast_t* byte_ptr, lzws_symbol_t** source_ptr, size_t* source_length_ptr);
void lzws_compressor_write_byte(lzws_compressor_state_t* state_ptr, lzws_symbol_fast_t byte, lzws_symbol_t** destination_ptr, size_t* destination_length_ptr);

#endif // LZWS_COMPRESSOR_UTILS_H
