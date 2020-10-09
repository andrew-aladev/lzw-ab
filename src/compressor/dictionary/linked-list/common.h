// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_COMPRESSOR_DICTIONARY_LINKED_LIST_COMMON_H)
#define LZWS_COMPRESSOR_DICTIONARY_LINKED_LIST_COMMON_H

#include "../../common.h"

typedef struct {
  lzws_code_t* first_child_codes;
  lzws_code_t* next_sibling_codes;
  lzws_byte_t* last_symbol_by_codes;

  lzws_code_fast_t first_child_codes_offset;
  lzws_code_fast_t next_sibling_codes_offset;
} lzws_compressor_dictionary_t;

#endif // LZWS_COMPRESSOR_DICTIONARY_LINKED_LIST_COMMON_H
