// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "main.h"

#include <stdarg.h>

#include "../macro.h"

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

lzws_result_t lzws_bigint_add_uint32(lzws_bigint_t* bigint_ptr, uint32_t addition, bool LZWS_UNUSED(quiet))
{
  mpz_add_ui(*bigint_ptr, *bigint_ptr, addition);

  return 0;
}

lzws_result_t lzws_bigint_multiply_by_uint32(lzws_bigint_t* bigint_ptr, uint32_t multiplicator, bool LZWS_UNUSED(quiet))
{
  mpz_mul_ui(*bigint_ptr, *bigint_ptr, multiplicator);

  return 0;
}

lzws_bigint_compare_result_t lzws_bigint_compare(lzws_bigint_t* first_bigint_ptr, lzws_bigint_t* second_bigint_ptr)
{
  int result = mpz_cmp(*first_bigint_ptr, *second_bigint_ptr);
  if (result == 0) {
    return LZWS_BIGINT_COMPARE_EQUALS;
  } else if (result < 0) {
    return LZWS_BIGINT_COMPARE_LESS_THAN;
  } else {
    return LZWS_BIGINT_COMPARE_GREATER_THAN;
  }
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
