// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "main.h"

#include <stdarg.h>

#include "../log.h"

lzws_result_t lzws_bigint_initialize_multiple(bool quiet, lzws_bigint_t* bigint_ptr, ...)
{
  lzws_bigint_t* current_bigint_ptr = bigint_ptr;

  va_list args;
  va_start(args, bigint_ptr);

  while (current_bigint_ptr != NULL) {
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

    current_bigint_ptr = va_arg(args, lzws_bigint_t*);
  }

  va_end(args);

  return 0;
}

lzws_result_t lzws_bigint_add_uint32(lzws_bigint_t* bigint_ptr, uint32_t addition, bool quiet)
{
  lzws_bigint_t bigint_addition;

  if (mp_init(&bigint_addition) != MP_OKAY) {
    if (!quiet) {
      LZWS_LOG_ERROR("failed to initialize bigint");
    }

    return LZWS_BIGINT_INITIALIZE_FAILED;
  }

  mp_set_u32(&bigint_addition, addition);
  bool failed = mp_add(bigint_ptr, &bigint_addition, bigint_ptr) != MP_OKAY;

  mp_clear(&bigint_addition);

  if (failed) {
    if (!quiet) {
      LZWS_LOG_ERROR("failed to add int %" PRIu32 " to bigint", addition);
    }

    return LZWS_BIGINT_ADD_FAILED;
  }

  return 0;
}

lzws_result_t lzws_bigint_multiply_by_uint32(lzws_bigint_t* bigint_ptr, uint32_t multiplicator, bool quiet)
{
  lzws_bigint_t bigint_multiplicator;

  if (mp_init(&bigint_multiplicator) != MP_OKAY) {
    if (!quiet) {
      LZWS_LOG_ERROR("failed to initialize bigint");
    }

    return LZWS_BIGINT_INITIALIZE_FAILED;
  }

  mp_set_u32(&bigint_multiplicator, multiplicator);
  bool failed = mp_mul(bigint_ptr, &bigint_multiplicator, bigint_ptr) != MP_OKAY;

  mp_clear(&bigint_multiplicator);

  if (failed) {
    if (!quiet) {
      LZWS_LOG_ERROR("failed to multiply bigint by int %" PRIu32, multiplicator);
    }

    return LZWS_BIGINT_MULTIPLY_FAILED;
  }

  return 0;
}

lzws_bigint_compare_result_t lzws_bigint_compare(lzws_bigint_t* first_bigint_ptr, lzws_bigint_t* second_bigint_ptr)
{
  mp_ord result = mp_cmp(first_bigint_ptr, second_bigint_ptr);
  switch (result) {
    case MP_EQ:
      return LZWS_BIGINT_COMPARE_EQUALS;
    case MP_LT:
      return LZWS_BIGINT_COMPARE_LESS_THAN;
    default:
      return LZWS_BIGINT_COMPARE_GREATER_THAN;
  }
}

void lzws_bigint_clear_multiple(lzws_bigint_t* bigint_ptr, ...)
{
  lzws_bigint_t* current_bigint_ptr = bigint_ptr;

  va_list args;
  va_start(args, bigint_ptr);

  while (current_bigint_ptr != NULL) {
    mp_clear(current_bigint_ptr);

    current_bigint_ptr = va_arg(args, lzws_bigint_t*);
  }

  va_end(args);
}
