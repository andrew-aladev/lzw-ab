// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_COMMON_H)
#define LZWS_COMMON_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

#include "config.h"
#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

// We are working for POSIX systems and Windows (MinGW) only.
// These systems require CHAR_BIT to be 8.
// We won't use "least" types from "stdint.h".

// We will use precise types for source, destination, dictionary and constants to optimize memory consumption.
// We will try to use fast types in all other cases.

typedef uint_fast8_t lzws_result_t;

typedef uint8_t      lzws_byte_t;
typedef uint_fast8_t lzws_byte_fast_t;

typedef uint16_t      lzws_code_t;
typedef uint_fast16_t lzws_code_fast_t;

#define LZWS_CODE_FORMAT      "%" PRIu16
#define LZWS_FAST_CODE_FORMAT "%" PRIuFAST16

// External programs requires 2 byte magic header.
// For example:
// https://github.com/apache/httpd/blob/b4ee699ea355271bb560248e5f89cca72623a0cf/modules/metadata/mod_mime_magic.c#L2060
#define LZWS_FIRST_MAGIC_HEADER_BYTE  0x1f // "\037" in base-8 number system.
#define LZWS_SECOND_MAGIC_HEADER_BYTE 0x9d // "\235" in base-8 number system.

#define LZWS_LOWEST_MAX_CODE_BIT_LENGTH  9
#define LZWS_BIGGEST_MAX_CODE_BIT_LENGTH 16
#define LZWS_MAX_CODE_BIT_MASK           0x1f // Max code bits are the last bits, biggest value (16) requires 5 bits.
#define LZWS_BLOCK_MODE                  0x80

#define LZWS_ALPHABET_LENGTH             256
#define LZWS_ALPHABET_BIT_LENGTH         8
#define LZWS_CLEAR_CODE                  256
#define LZWS_RATIO_SOURCE_CHECKPOINT_GAP 10000

// Clear code can be used only in block mode.
#define LZWS_FIRST_FREE_CODE               256
#define LZWS_FIRST_FREE_CODE_IN_BLOCK_MODE 257

// We can use any value between 0 and alphabet length - 1 as undefined free code.
#define LZWS_UNDEFINED_FREE_CODE 0

LZWS_EXPORT inline lzws_code_fast_t lzws_get_first_free_code(bool block_mode)
{
  return block_mode ? LZWS_FIRST_FREE_CODE_IN_BLOCK_MODE : LZWS_FIRST_FREE_CODE;
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LZWS_COMMON_H
