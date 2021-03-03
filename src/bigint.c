// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "bigint.h"

#include <stdarg.h>

#include "log.h"
#include "macro.h"

// Implementations for different bignum libraries.

lzws_result_t lzws_bigint_initialize_multiple(bool LZWS_UNUSED(quiet), lzws_bigint_t* bigint_ptr, ...)
{
  lzws_bigint_t* current_bigint_ptr = bigint_ptr;

  va_list args;
  va_start(args, bigint_ptr);

  while (current_bigint_ptr != NULL) {
#if defined(LZWS_BIGNUM_LIBRARY_GMP)
    mpz_init(*current_bigint_ptr);

#elif defined(LZWS_BIGNUM_LIBRARY_TOMMATH)
    if (mp_init(current_bigint_ptr) != MP_OKAY) {
      if (!quiet) {
        LZWS_LOG_ERROR("failed to initialize bigint");
      }

      // We need to clear all initialized ints.
      lzws_bigint_t* initialized_bigint_ptr = bigint_ptr;

      va_list initialized_args;
      va_start(initialized_args, bigint_ptr);

      while (initialized_bigint_ptr != current_bigint_ptr) {
        mp_clear(initialized_bigint_ptr);
        initialized_bigint_ptr = va_arg(initialized_args, lzws_bigint_t*);
      }

      va_end(initialized_args);

      return LZWS_BIGINT_INITIALIZE_FAILED;
    }
#endif

    current_bigint_ptr = va_arg(args, lzws_bigint_t*);
  }

  va_end(args);

  return 0;
}

lzws_result_t lzws_bigint_add_uint32(lzws_bigint_t* bigint_ptr, uint32_t addition, bool LZWS_UNUSED(quiet))
{
#if defined(LZWS_BIGNUM_LIBRARY_GMP)
  mpz_add_ui(*bigint_ptr, *bigint_ptr, addition);

#elif defined(LZWS_BIGNUM_LIBRARY_TOMMATH)
  lzws_bigint_t bigint_addition;

  if (mp_init(&bigint_addition) != MP_OKAY) {
    if (!quiet) {
      LZWS_LOG_ERROR("failed to initialize bigint");
    }

    return LZWS_BIGINT_INITIALIZE_FAILED;
  }

  mp_set_u32(&bigint_addition, addition);
  bool failed = mp_add(bigint_ptr, &bigint_addition, bigint_ptr) != MP_OKAY;

  mp_clear(bigint_addition);

  if (failed) {
    if (!quiet) {
      LZWS_LOG_ERROR("failed to add int %" PRIu32 " to bigint", addition);
    }

    return LZWS_BIGINT_ADD_FAILED;
  }
#endif

  return 0;
}

void lzws_bigint_clear_multiple(lzws_bigint_t* bigint_ptr, ...)
{
  lzws_bigint_t* current_bigint_ptr = bigint_ptr;

  va_list args;
  va_start(args, bigint_ptr);

  while (current_bigint_ptr != NULL) {
#if defined(LZWS_BIGNUM_LIBRARY_GMP)
    mpz_clear(*current_bigint_ptr);
#elif defined(LZWS_BIGNUM_LIBRARY_TOMMATH)
    mp_clear(current_bigint_ptr);
#endif

    current_bigint_ptr = va_arg(args, lzws_bigint_t*);
  }

  va_end(args);
}
