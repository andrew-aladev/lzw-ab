// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_COMPRESSOR_COMMON_H)
#define LZWS_COMPRESSOR_COMMON_H

#include "../common.h"

typedef struct
{
  bool             without_magic_header;
  lzws_byte_fast_t max_code_bit_length;
  bool             block_mode;
  bool             msb;
  bool             unaligned_bit_groups;
  bool             quiet;
} lzws_compressor_options_t;

extern const lzws_compressor_options_t LZWS_COMPRESSOR_DEFAULT_OPTIONS;

// Possible results:
enum
{
  LZWS_COMPRESSOR_ALLOCATE_FAILED = 1,
  LZWS_COMPRESSOR_INVALID_MAX_CODE_BIT_LENGTH,
  LZWS_COMPRESSOR_NEEDS_MORE_SOURCE,
  LZWS_COMPRESSOR_NEEDS_MORE_DESTINATION,
  LZWS_COMPRESSOR_UNKNOWN_STATUS
};

// Code can have any value, but next code will always be > alphabet length.
// So we can use zero as undefined next code.
#define LZWS_COMPRESSOR_UNDEFINED_NEXT_CODE                     0
#define LZWS_COMPRESSOR_UNDEFINED_NEXT_CODE_HAS_IDENTICAL_BYTES true

#endif // LZWS_COMPRESSOR_COMMON_H
