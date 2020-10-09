// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_LOG_H)
#define LZWS_LOG_H

#include <stdio.h>

// Relative file path can be provided during each source file compilation.
#ifndef __RELATIVE_FILE_PATH__
#define __RELATIVE_FILE_PATH__ __FILE__
#endif

#define _LZWS_LOG(target, ...)                                                \
  fprintf(target, "%s %s:%u - ", __func__, __RELATIVE_FILE_PATH__, __LINE__); \
  fprintf(target, __VA_ARGS__);                                               \
  fputs("\n", target);

#define LZWS_LOG_ERROR(...) _LZWS_LOG(stderr, __VA_ARGS__)

#endif // LZWS_LOG_H
