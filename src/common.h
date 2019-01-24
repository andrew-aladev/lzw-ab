// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_COMMON_H)
#define LZWS_COMMON_H

#include <stdbool.h>
#include <stdint.h>

#undef LZWS_INLINE
#if defined(LZWS_COMMON_C)
#define LZWS_INLINE
#else
#define LZWS_INLINE inline
#endif

// We are working for POSIX systems only.
// POSIX system requires CHAR_BIT to be 8.
// We won't use "least" types from "stdint.h".

// We will use precise types for source, destination, dictionary and constants to optimize memory consumption.
// We will try to use fast types in all other cases.

typedef uint_fast8_t lzws_result_t;

typedef uint16_t      lzws_code_t;
typedef uint_fast16_t lzws_code_fast_t;

#define CODE_FORMAT "%u"
#define FAST_CODE_FORMAT "%lu"

// External programs requires 2 byte magic header.
// For example: https://github.com/apache/httpd/blob/trunk/modules/metadata/mod_mime_magic.c#L2060
#define LZWS_FIRST_MAGIC_HEADER_BYTE 0x1f  // "\037" in base-8 number system.
#define LZWS_SECOND_MAGIC_HEADER_BYTE 0x9d // "\235" in base-8 number system.

#define LZWS_LOWEST_MAX_CODE_BIT_LENGTH 9
#define LZWS_BIGGEST_MAX_CODE_BIT_LENGTH 16
#define LZWS_MAX_CODE_BIT_MASK 0x1f // Max code bits are the last bits, biggest value (16) requires 5 bits.
#define LZWS_BLOCK_MODE 0x80

#define LZWS_ALPHABET_LENGTH 256
#define LZWS_ALPHABET_BIT_LENGTH 8
#define LZWS_CLEAR_CODE 256
#define LZWS_RATIO_SOURCE_CHECKPOINT_GAP 10000

// Clear code can be used only in block mode.
#define LZWS_INITIAL_USED_CODE 255
#define LZWS_INITIAL_USED_CODE_IN_BLOCK_MODE 256

LZWS_INLINE lzws_code_fast_t lzws_get_initial_used_code(bool block_mode) {
  return block_mode ? LZWS_INITIAL_USED_CODE_IN_BLOCK_MODE : LZWS_INITIAL_USED_CODE;
}

#endif // LZWS_COMMON_H
