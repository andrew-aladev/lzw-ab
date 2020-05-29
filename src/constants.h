// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_CONSTANTS_H)
#define LZWS_CONSTANTS_H

#include "common.h"

// Standard table to reverse single byte.
extern const lzws_byte_t LZWS_BYTES_WITH_REVERSED_BITS[256];

// Algorithm will try to access ((2 ** 0) - 1) - ((2 ** 16) - 1) only.
extern const lzws_code_t LZWS_MASKS_FOR_LAST_BITS[17];

#endif // LZWS_CONSTANTS_H
