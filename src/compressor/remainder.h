// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_COMPRESSOR_REMAINDER_H)
#define LZWS_COMPRESSOR_REMAINDER_H

#include "state.h"

lzws_result_t lzws_compressor_write_remainder_before_read_next_symbol(lzws_compressor_state_t* state_ptr,
                                                                      lzws_byte_t**            destination_ptr,
                                                                      size_t*                  destination_length_ptr);
lzws_result_t lzws_compressor_write_remainder_before_current_code(lzws_compressor_state_t* state_ptr,
                                                                  lzws_byte_t**            destination_ptr,
                                                                  size_t*                  destination_length_ptr);
lzws_result_t lzws_compressor_flush_remainder_before_current_code(lzws_compressor_state_t* state_ptr,
                                                                  lzws_byte_t**            destination_ptr,
                                                                  size_t*                  destination_length_ptr);
lzws_result_t lzws_compressor_flush_remainder(lzws_compressor_state_t* state_ptr, lzws_byte_t** destination_ptr,
                                              size_t* destination_length_ptr);

#endif // LZWS_COMPRESSOR_REMAINDER_H
