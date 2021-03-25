// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LIBRARY_H)
#define LIBRARY_H

#include "export.h"

#if defined(__cplusplus)
extern "C" {
#endif

inline int inline_function()
{
  return 0;
}

CHECK_EXPORT int function();

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // LIBRARY_H
