// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_COMPRESSOR_DICTIONARY_WRAPPER_H)
#define LZWS_COMPRESSOR_DICTIONARY_WRAPPER_H

#include "../../config.h"

#if defined(LZWS_COMPRESSOR_DICTIONARY_LINKED_LIST)
#include "linked-list/wrapper.h"
#elif defined(LZWS_COMPRESSOR_DICTIONARY_SPARSE_ARRAY)
#include "sparse-array/wrapper.h"
#endif

#endif // LZWS_COMPRESSOR_DICTIONARY_WRAPPER_H
