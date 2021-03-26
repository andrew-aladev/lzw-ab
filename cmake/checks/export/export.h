// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(EXPORT_H)
#define EXPORT_H

#include "config.h"

#if defined(EXPORT_STATIC_FLAG)

#define EXPORT
#define NO_EXPORT

#else

#if !defined(EXPORT)
#if defined(EXPORT_MODE_DLL)

#if defined(EXPORT_BUILD_FLAG)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif

#elif defined(EXPORT_MODE_VISIBILITY)
#define EXPORT __attribute__((visibility("default")))
#else
#define EXPORT
#endif
#endif // EXPORT

#if !defined(NO_EXPORT)
#if defined(EXPORT_MODE_DLL)
#define NO_EXPORT
#elif defined(EXPORT_MODE_VISIBILITY)
#define NO_EXPORT __attribute__((visibility("hidden")))
#else
#define NO_EXPORT
#endif
#endif // NO_EXPORT

#endif // EXPORT_STATIC_FLAG

#endif // EXPORT_H
