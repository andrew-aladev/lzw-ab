// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_COMPRESSOR_DICTIONARY_SPARSE_ARRAY_TYPE_H)
#define LZWS_COMPRESSOR_DICTIONARY_SPARSE_ARRAY_TYPE_H

#include "../../../common.h"

typedef struct {
  lzws_code_fast_t codes_length_offset;
  lzws_code_t*     next_codes;
} lzws_compressor_dictionary_t;

#endif // LZWS_COMPRESSOR_DICTIONARY_SPARSE_ARRAY_TYPE_H
