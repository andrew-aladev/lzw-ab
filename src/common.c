// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "common.h"

LZWS_EXPORT extern inline lzws_code_fast_t lzws_get_first_free_code(bool block_mode);
