// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(CONTENT_H)
#define CONTENT_H

#include "export.h"

#if defined(__cplusplus)
extern "C" {
#endif

EXPORT extern const int    data;
NO_EXPORT extern const int hidden_data;

EXPORT inline int inline_function()
{
  return 0;
}

NO_EXPORT inline int hidden_inline_function()
{
  return 1;
}

EXPORT int    function();
NO_EXPORT int hidden_function();

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // CONTENT_H
