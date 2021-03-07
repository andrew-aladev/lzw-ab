// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "tommath_int.h"

#include <stdarg.h>

#include "../log.h"

// -- error --

static inline lzws_result_t get_error_result(mp_err mp_result, bool quiet)
{
  if (mp_result == MP_MEM) {
    if (!quiet) {
      LZWS_LOG_ERROR("failed to allocate bigint");
    }

    return LZWS_BIGINT_ALLOCATE_FAILED;
  }

  if (!quiet) {
    LZWS_LOG_ERROR("unknown bigint error: %d", mp_result);
  }

  return LZWS_BIGINT_UNKNOWN_ERROR;
}

// -- implementation --

lzws_result_t lzws_bigint_initialize_multiple(bool quiet, lzws_bigint_t* bigint_ptr, ...)
{
  lzws_result_t  result             = 0;
  lzws_bigint_t* current_bigint_ptr = bigint_ptr;

  va_list args;
  va_start(args, bigint_ptr);

  while (current_bigint_ptr != NULL) {
    mp_err mp_result = mp_init(current_bigint_ptr);

    if (mp_result != MP_OKAY) {
      if (!quiet) {
        LZWS_LOG_ERROR("failed to initialize bigint");
      }

      result = get_error_result(mp_result, quiet);

      // We need to clear all initialized ints.
      lzws_bigint_t* initialized_bigint_ptr = bigint_ptr;

      va_list initialized_args;
      va_start(initialized_args, bigint_ptr);

      while (initialized_bigint_ptr != current_bigint_ptr) {
        mp_clear(initialized_bigint_ptr);

        initialized_bigint_ptr = va_arg(initialized_args, lzws_bigint_t*);
      }

      va_end(initialized_args);

      break;
    }

    current_bigint_ptr = va_arg(args, lzws_bigint_t*);
  }

  va_end(args);

  return result;
}

lzws_result_t lzws_bigint_add_uint32(lzws_bigint_t* bigint_ptr, uint32_t addition, bool quiet)
{
  lzws_bigint_t addition_bigint;

  mp_err mp_result = mp_init(&addition_bigint);
  if (mp_result != MP_OKAY) {
    if (!quiet) {
      LZWS_LOG_ERROR("failed to initialize bigint");
    }

    return get_error_result(mp_result, quiet);
  }

  mp_set_u32(&addition_bigint, addition);
  mp_result = mp_add(bigint_ptr, &addition_bigint, bigint_ptr) != MP_OKAY;

  mp_clear(&addition_bigint);

  if (mp_result != MP_OKAY) {
    if (!quiet) {
      LZWS_LOG_ERROR("failed to add int %" PRIu32 " to bigint", addition);
    }

    return get_error_result(mp_result, quiet);
  }

  return 0;
}

lzws_result_t lzws_bigint_multiply_by_uint32(lzws_bigint_t* bigint_ptr, uint32_t multiplicator, bool quiet)
{
  lzws_bigint_t multiplicator_bigint;

  mp_err mp_result = mp_init(&multiplicator_bigint);
  if (mp_result != MP_OKAY) {
    if (!quiet) {
      LZWS_LOG_ERROR("failed to initialize bigint");
    }

    return get_error_result(mp_result, quiet);
  }

  mp_set_u32(&multiplicator_bigint, multiplicator);
  mp_result = mp_mul(bigint_ptr, &multiplicator_bigint, bigint_ptr) != MP_OKAY;

  mp_clear(&multiplicator_bigint);

  if (mp_result != MP_OKAY) {
    if (!quiet) {
      LZWS_LOG_ERROR("failed to multiply bigint by int %" PRIu32, multiplicator);
    }

    return get_error_result(mp_result, quiet);
  }

  return 0;
}

lzws_bigint_compare_result_t lzws_bigint_compare(lzws_bigint_t* first_bigint_ptr, lzws_bigint_t* second_bigint_ptr)
{
  mp_ord mp_result = mp_cmp(first_bigint_ptr, second_bigint_ptr);
  switch (mp_result) {
    case MP_EQ:
      return LZWS_BIGINT_COMPARE_EQUALS;
    case MP_LT:
      return LZWS_BIGINT_COMPARE_LESS_THAN;
    default:
      return LZWS_BIGINT_COMPARE_GREATER_THAN;
  }
}

lzws_result_t lzws_bigint_compare_with_uint32(
  lzws_bigint_t*                first_bigint_ptr,
  uint32_t                      second_int,
  bool                          quiet,
  lzws_bigint_compare_result_t* compare_result_ptr)
{
  lzws_bigint_t second_bigint;

  mp_err mp_result = mp_init(&second_bigint);
  if (mp_result != MP_OKAY) {
    if (!quiet) {
      LZWS_LOG_ERROR("failed to initialize bigint");
    }

    return get_error_result(mp_result, quiet);
  }

  mp_set_u32(&second_bigint, second_int);
  *compare_result_ptr = lzws_bigint_compare(first_bigint_ptr, &second_bigint);

  mp_clear(&second_bigint);

  return 0;
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
