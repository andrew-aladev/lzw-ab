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

typedef uint8_t  lzws_result_t;
typedef uint16_t lzws_code_t;

// External programs requires 2 byte magic header.
// For example: https://github.com/apache/httpd/blob/trunk/modules/metadata/mod_mime_magic.c#L2060
#define LZWS_MAGIC_HEADER_BYTE_0 0x1f // "\037" in base-8 number system.
#define LZWS_MAGIC_HEADER_BYTE_1 0x9d // "\235" in base-8 number system.

#define LZWS_LOWEST_MAX_CODE_BITS 9
#define LZWS_BIGGEST_MAX_CODE_BITS 16
#define LZWS_MAX_CODE_BITS_MASK 0x1f // "max_bits" are the last bits, biggest value (16) requires 5 bits.
#define LZWS_BLOCK_MODE 0x80

// Code can have any value, but next code will always be > 255.
// We will use 0 as undefined next code.
#define LZWS_UNDEFINED_NEXT_CODE 0
#define LZWS_CLEAR_CODE 256

// Clear code can be used only in block mode.
#define LZWS_INITIAL_USED_CODE 255
#define LZWS_INITIAL_USED_CODE_IN_BLOCK_MODE 256

LZWS_INLINE lzws_code_t lzws_get_initial_used_code(bool block_mode) {
  return block_mode ? LZWS_INITIAL_USED_CODE_IN_BLOCK_MODE : LZWS_INITIAL_USED_CODE;
}

#endif // LZWS_COMMON_H