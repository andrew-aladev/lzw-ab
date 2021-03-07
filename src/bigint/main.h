// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_BIGINT_MAIN_H)
#define LZWS_BIGINT_MAIN_H

#include "../config.h"

#if defined(LZWS_BIGNUM_LIBRARY_GMP)
#include "gmp_int.h"
#elif defined(LZWS_BIGNUM_LIBRARY_TOMMATH)
#include "tommath_int.h"
#endif

#endif // LZWS_BIGINT_MAIN_H
