// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_BIGINT_H)
#define LZWS_BIGINT_H

#include "config.h"

#if defined(LZWS_BIGNUM_LIBRARY_GMP)
#include <gmp.h>
#elif defined(LZWS_BIGNUM_LIBRARY_TOMMATH)
#include <tommath.h>
#endif

#include "common.h"

#if defined(LZWS_BIGNUM_LIBRARY_GMP)
typedef mpz_t lzws_bigint_t;
#elif defined(LZWS_BIGNUM_LIBRARY_TOMMATH)
typedef mp_int lzws_bigint_t;
#endif

// Possible results:
enum
{
  LZWS_BIGINT_INITIALIZE_FAILED = 1,
  LZWS_BIGINT_ADD_FAILED,
  LZWS_BIGINT_MULTIPLY_FAILED,
};

lzws_result_t lzws_bigint_initialize_multiple(bool quiet, lzws_bigint_t* bigint_ptr, ...);
lzws_result_t lzws_bigint_add_uint32(lzws_bigint_t* bigint_ptr, uint32_t addition, bool quiet);
void          lzws_bigint_clear_multiple(lzws_bigint_t* bigint_ptr, ...);

#endif // LZWS_BIGINT_H
