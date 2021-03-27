// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_EXPORT_H)
#define LZWS_EXPORT_H

#include "config.h"

#if defined(LZWS_EXPORT_STATIC_FLAG)

#define LZWS_EXPORT
#define LZWS_NO_EXPORT

#else

#if defined(LZWS_EXPORT_MODE_DLL)

#if defined(LZWS_EXPORT_LIBRARY_FLAG)
#define LZWS_EXPORT __declspec(dllexport)
#else
#define LZWS_EXPORT __declspec(dllimport)
#endif

#elif defined(LZWS_EXPORT_MODE_VISIBILITY)
#define LZWS_EXPORT __attribute__((visibility("default")))
#else
#define LZWS_EXPORT
#endif

#if defined(LZWS_EXPORT_MODE_DLL)
#define LZWS_NO_EXPORT
#elif defined(LZWS_EXPORT_MODE_VISIBILITY)
#define LZWS_NO_EXPORT __attribute__((visibility("hidden")))
#else
#define LZWS_NO_EXPORT
#endif

#endif // LZWS_EXPORT_STATIC_FLAG

#endif // LZWS_EXPORT_H
