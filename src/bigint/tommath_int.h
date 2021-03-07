// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_BIGINT_TOMMATH_INT_H)
#define LZWS_BIGINT_TOMMATH_INT_H

#include <tommath.h>

#include "../common.h"
#include "common.h"

typedef mp_int lzws_bigint_t;

lzws_result_t lzws_bigint_initialize_multiple(bool quiet, lzws_bigint_t* bigint_ptr, ...);

lzws_result_t lzws_bigint_add_uint32(
  lzws_bigint_t* destination_bigint_ptr,
  lzws_bigint_t* source_bigint_ptr,
  uint32_t       addition,
  bool           quiet);

lzws_result_t lzws_bigint_multiply_by_uint32(
  lzws_bigint_t* destination_bigint_ptr,
  lzws_bigint_t* source_bigint_ptr,
  uint32_t       multiplicator,
  bool           quiet);

lzws_bigint_compare_result_t lzws_bigint_compare(lzws_bigint_t* first_bigint_ptr, lzws_bigint_t* second_bigint_ptr);

lzws_result_t lzws_bigint_compare_with_uint32(
  lzws_bigint_t*                first_bigint_ptr,
  uint32_t                      second_int,
  bool                          quiet,
  lzws_bigint_compare_result_t* compare_result_ptr);

void lzws_bigint_clear_multiple(lzws_bigint_t* bigint_ptr, ...);

#endif // LZWS_BIGINT_TOMMATH_INT_H
