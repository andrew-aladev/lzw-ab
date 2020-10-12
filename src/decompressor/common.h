// LZW streaming decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_DECOMPRESSOR_COMMON_H)
#define LZWS_DECOMPRESSOR_COMMON_H

#include "../common.h"

typedef struct
{
  bool without_magic_header;
  bool msb;
  bool unaligned_bit_groups;
  bool quiet;
} lzws_decompressor_options_t;

extern const lzws_decompressor_options_t LZWS_DECOMPRESSOR_DEFAULT_OPTIONS;

// Possible results:
enum
{
  LZWS_DECOMPRESSOR_ALLOCATE_FAILED = 1,
  LZWS_DECOMPRESSOR_INVALID_MAGIC_HEADER,
  LZWS_DECOMPRESSOR_INVALID_MAX_CODE_BIT_LENGTH,
  LZWS_DECOMPRESSOR_NEEDS_MORE_SOURCE,
  LZWS_DECOMPRESSOR_NEEDS_MORE_DESTINATION,
  LZWS_DECOMPRESSOR_CORRUPTED_SOURCE,
  LZWS_DECOMPRESSOR_UNKNOWN_STATUS
};

#endif // LZWS_DECOMPRESSOR_COMMON_H
