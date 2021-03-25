// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(CONTENT_H)
#define CONTENT_H

#include "export.h"

#if defined(__cplusplus)
extern "C" {
#endif

EXPORT_DATA extern const int data;

EXPORT_FUNCTION inline int inline_function()
{
  return 0;
}

EXPORT_FUNCTION int function();

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // CONTENT_H
