// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "content.h"

EXPORT const int    data        = 0;
NO_EXPORT const int hidden_data = 1;

EXPORT extern inline int    inline_function(void);
NO_EXPORT extern inline int hidden_inline_function(void);

EXPORT int function(void)
{
  return 0;
}

NO_EXPORT int hidden_function(void)
{
  return 1;
}
