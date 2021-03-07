// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "gmp_int.h"

#include <stdarg.h>

lzws_result_t lzws_bigint_initialize_multiple(bool LZWS_UNUSED(quiet), lzws_bigint_t* bigint_ptr, ...)
{
  lzws_bigint_t* current_bigint_ptr = bigint_ptr;

  va_list args;
  va_start(args, bigint_ptr);

  while (current_bigint_ptr != NULL) {
    mpz_init(*current_bigint_ptr);

    current_bigint_ptr = va_arg(args, lzws_bigint_t*);
  }

  va_end(args);

  return 0;
}

extern inline lzws_result_t
  lzws_bigint_add_uint32(lzws_bigint_t* bigint_ptr, uint32_t addition, bool LZWS_UNUSED(quiet));

extern inline lzws_result_t
  lzws_bigint_multiply_by_uint32(lzws_bigint_t* bigint_ptr, uint32_t multiplicator, bool LZWS_UNUSED(quiet));

static inline lzws_bigint_compare_result_t get_compare_result(int mpz_result)
{
  if (mpz_result == 0) {
    return LZWS_BIGINT_COMPARE_EQUALS;
  } else if (mpz_result < 0) {
    return LZWS_BIGINT_COMPARE_LESS_THAN;
  } else {
    return LZWS_BIGINT_COMPARE_GREATER_THAN;
  }
}

lzws_bigint_compare_result_t lzws_bigint_compare(lzws_bigint_t* first_bigint_ptr, lzws_bigint_t* second_bigint_ptr)
{
  int mpz_result = mpz_cmp(*first_bigint_ptr, *second_bigint_ptr);

  return get_compare_result(mpz_result);
}

lzws_result_t lzws_bigint_compare_with_uint32(
  lzws_bigint_t*                first_bigint_ptr,
  uint32_t                      second_int,
  bool                          LZWS_UNUSED(quiet),
  lzws_bigint_compare_result_t* compare_result_ptr)
{
  int mpz_result = mpz_cmp_ui(*first_bigint_ptr, second_int);

  *compare_result_ptr = get_compare_result(mpz_result);

  return 0;
}

void lzws_bigint_clear_multiple(lzws_bigint_t* bigint_ptr, ...)
{
  lzws_bigint_t* current_bigint_ptr = bigint_ptr;

  va_list args;
  va_start(args, bigint_ptr);

  while (current_bigint_ptr != NULL) {
    mpz_clear(*current_bigint_ptr);

    current_bigint_ptr = va_arg(args, lzws_bigint_t*);
  }

  va_end(args);
}
