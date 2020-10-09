// LZW streaming decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "common.h"

const lzws_decompressor_options_t LZWS_DECOMPRESSOR_DEFAULT_OPTIONS = {
  .without_magic_header = false, .block_mode = true, .msb = false, .unaligned_bit_groups = false, .quiet = false};
