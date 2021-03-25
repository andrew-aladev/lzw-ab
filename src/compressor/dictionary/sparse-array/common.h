// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_COMPRESSOR_DICTIONARY_SPARSE_ARRAY_COMMON_H)
#define LZWS_COMPRESSOR_DICTIONARY_SPARSE_ARRAY_COMMON_H

#include "../../common.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef uint32_t lzws_compressor_dictionary_used_index_t;

typedef struct
{
  lzws_code_t*                             next_codes;
  lzws_compressor_dictionary_used_index_t* used_indexes;

  lzws_code_fast_t next_codes_offset;
  lzws_code_fast_t used_indexes_offset;
} lzws_compressor_dictionary_t;

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // LZWS_COMPRESSOR_DICTIONARY_SPARSE_ARRAY_COMMON_H
