// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(EXPORT_H)
#define EXPORT_H

#include "config.h"

#if defined(EXPORT_STATIC_FLAG)

#define EXPORT_FUNCTION
#define EXPORT_DATA

#else

#if !defined(EXPORT_FUNCTION)
#if defined(EXPORT_MODE_DLL)

#if defined(EXPORT_BUILD_FLAG)
#define EXPORT_FUNCTION __declspec(dllexport)
#else
#define EXPORT_FUNCTION __declspec(dllimport)
#endif

#elsif defined(EXPORT_MODE_VISIBILITY)
#define EXPORT_FUNCTION __attribute__((visibility("default")))
#else
#define EXPORT_FUNCTION
#endif
#endif // EXPORT_FUNCTION

#if !defined(EXPORT_DATA)
#if defined(EXPORT_MODE_DLL) || defined(EXPORT_MODE_DLL_AUTO)

#if defined(EXPORT_BUILD_FLAG)
#define EXPORT_DATA __declspec(dllexport)
#else
#define EXPORT_DATA __declspec(dllimport)
#endif

#elsif defined(EXPORT_MODE_VISIBILITY)
#define EXPORT_DATA __attribute__((visibility("default")))
#else
#define EXPORT_DATA
#endif
#endif // EXPORT_DATA

#endif // EXPORT_STATIC_FLAG

#endif // EXPORT_H
