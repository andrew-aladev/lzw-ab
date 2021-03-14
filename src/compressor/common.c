// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "common.h"

LZWS_EXPORT const lzws_compressor_options_t LZWS_COMPRESSOR_DEFAULT_OPTIONS = {
  .without_magic_header = false,
  .max_code_bit_length  = LZWS_BIGGEST_MAX_CODE_BIT_LENGTH,
  .block_mode           = true,
  .msb                  = false,
  .unaligned_bit_groups = false,
  .quiet                = false};
