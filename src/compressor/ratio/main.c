// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "main.h"

#include "../common.h"

// -- error --

static inline lzws_result_t get_error_result(lzws_result_t bigint_result)
{
  if (bigint_result == LZWS_BIGINT_ALLOCATE_FAILED) {
    return LZWS_COMPRESSOR_ALLOCATE_FAILED;
  }

  return LZWS_COMPRESSOR_UNKNOWN_ERROR;
}

// -- implementation --

lzws_result_t lzws_compressor_initialize_ratio(lzws_compressor_ratio_t* ratio_ptr, bool quiet)
{
  lzws_result_t bigint_result =
    lzws_bigint_initialize_multiple(quiet, &ratio_ptr->source_length, &ratio_ptr->destination_length, NULL);

  if (bigint_result != 0) {
    return get_error_result(bigint_result);
  }

  ratio_ptr->new_source_length      = 0;
  ratio_ptr->new_destination_length = 0;

  return 0;
}

extern inline void lzws_compressor_add_source_symbol_to_ratio(lzws_compressor_ratio_t* ratio_ptr);
extern inline void lzws_compressor_add_destination_symbol_to_ratio(lzws_compressor_ratio_t* ratio_ptr);

lzws_result_t lzws_compressor_need_to_clear_by_ratio(lzws_compressor_ratio_t* ratio_ptr, bool quiet, bool* result_ptr)
{
  // We don't need to clear when checkpoint for source is not reached.

  if (ratio_ptr->new_source_length < LZWS_RATIO_SOURCE_CHECKPOINT_GAP) {
    *result_ptr = false;
    return 0;
  }

  // We don't need to clear when destination length equals to zero.
  // Source length won't be zero when destination length is not zero.

  lzws_bigint_compare_result_t destination_length_compared_with_zero;

  lzws_result_t bigint_result =
    lzws_bigint_compare_with_uint32(&ratio_ptr->destination_length, 0, quiet, &destination_length_compared_with_zero);

  if (bigint_result != 0) {
    return get_error_result(bigint_result);
  }

  if (destination_length_compared_with_zero == LZWS_BIGINT_COMPARE_EQUALS) {
    *result_ptr = false;
    return 0;
  }

  // We need to clear when "destination_length * new_source_length - source_length * new_destination_length" < 0.

  lzws_bigint_t destination_and_new_source, source_and_new_destination;

  bigint_result =
    lzws_bigint_initialize_multiple(quiet, &destination_and_new_source, &source_and_new_destination, NULL);

  if (bigint_result != 0) {
    return get_error_result(bigint_result);
  }

  bigint_result = lzws_bigint_multiply_by_uint32(
    &destination_and_new_source, &ratio_ptr->destination_length, ratio_ptr->new_source_length, quiet);

  if (bigint_result != 0) {
    lzws_bigint_clear_multiple(&destination_and_new_source, &source_and_new_destination, NULL);
    return get_error_result(bigint_result);
  }

  bigint_result = lzws_bigint_multiply_by_uint32(
    &source_and_new_destination, &ratio_ptr->source_length, ratio_ptr->new_destination_length, quiet);

  if (bigint_result != 0) {
    lzws_bigint_clear_multiple(&destination_and_new_source, &source_and_new_destination, NULL);
    return get_error_result(bigint_result);
  }

  bool need_to_clear_by_ratio =
    lzws_bigint_compare(&destination_and_new_source, &source_and_new_destination) == LZWS_BIGINT_COMPARE_LESS_THAN;

  lzws_bigint_clear_multiple(&destination_and_new_source, &source_and_new_destination, NULL);

  // Cleanup.

  lzws_result_t result = lzws_compressor_clear_ratio(ratio_ptr, quiet);
  if (result != 0) {
    return result;
  }

  *result_ptr = need_to_clear_by_ratio;

  return 0;
}

lzws_result_t lzws_compressor_clear_ratio(lzws_compressor_ratio_t* ratio_ptr, bool quiet)
{
  lzws_result_t bigint_result =
    lzws_bigint_add_uint32(&ratio_ptr->source_length, &ratio_ptr->source_length, ratio_ptr->new_source_length, quiet);

  if (bigint_result != 0) {
    return get_error_result(bigint_result);
  }

  bigint_result = lzws_bigint_add_uint32(
    &ratio_ptr->destination_length, &ratio_ptr->destination_length, ratio_ptr->new_destination_length, quiet);

  if (bigint_result != 0) {
    return get_error_result(bigint_result);
  }

  ratio_ptr->new_source_length      = 0;
  ratio_ptr->new_destination_length = 0;

  return 0;
}

extern inline void lzws_compressor_free_ratio(lzws_compressor_ratio_t* ratio_ptr);
