// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_TESTS_RANDOM_STRING_H)
#define LZWS_TESTS_RANDOM_STRING_H

#include <stdint.h>
#include <stdlib.h>

#include "../common.h"

#if defined(__cplusplus)
extern "C" {
#endif

LZWS_EXPORT void lzws_tests_set_random_string(char* buffer, size_t buffer_length);

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // LZWS_TESTS_RANDOM_STRING_H
