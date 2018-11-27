// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_COMPRESSOR_DICTIONARY_TRIE_ON_LINKED_LIST_TYPE_H)
#define LZWS_COMPRESSOR_DICTIONARY_TRIE_ON_LINKED_LIST_TYPE_H

#include "../../../common.h"

typedef struct {
  uint8_t initial_code_offset;

  lzws_code_t* first_child_codes;
  lzws_code_t* next_sibling_codes;
  uint8_t*     symbol_by_codes;
} lzws_compressor_dictionary_t;

#endif // LZWS_COMPRESSOR_DICTIONARY_TRIE_ON_LINKED_LIST_TYPE_H
