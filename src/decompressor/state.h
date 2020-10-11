// LZW streaming decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_DECOMPRESSOR_STATE_H)
#define LZWS_DECOMPRESSOR_STATE_H

#include "alignment/common.h"
#include "common.h"
#include "dictionary/common.h"

enum
{
  LZWS_DECOMPRESSOR_READ_MAGIC_HEADER = 1,
  LZWS_DECOMPRESSOR_READ_HEADER,
  LZWS_DECOMPRESSOR_ALLOCATE_DICTIONARY,
  LZWS_DECOMPRESSOR_READ_FIRST_CODE,
  LZWS_DECOMPRESSOR_READ_NEXT_CODE,
  LZWS_DECOMPRESSOR_WRITE_FIRST_SYMBOL,
  LZWS_DECOMPRESSOR_WRITE_SYMBOLS_FOR_CURRENT_CODE,
  LZWS_DECOMPRESSOR_READ_ALIGNMENT_BEFORE_FIRST_CODE,
  LZWS_DECOMPRESSOR_READ_ALIGNMENT_BEFORE_NEXT_CODE
};
typedef lzws_byte_fast_t lzws_decompressor_status_t;

typedef struct
{
  lzws_decompressor_options_t options;
  lzws_decompressor_status_t  status;

  lzws_code_fast_t first_free_code;
  lzws_code_fast_t max_code;

  lzws_code_fast_t free_code;
  lzws_byte_fast_t free_code_bit_length;
  lzws_code_fast_t max_free_code_for_bit_length;

  lzws_code_fast_t prefix_code;

  lzws_byte_fast_t remainder;
  lzws_byte_fast_t remainder_bit_length;

  lzws_decompressor_alignment_t  alignment;
  lzws_decompressor_dictionary_t dictionary;
} lzws_decompressor_state_t;

lzws_result_t lzws_decompressor_get_initial_state(
  lzws_decompressor_state_t**        state_ptr,
  const lzws_decompressor_options_t* options_ptr);

void lzws_decompressor_reset_last_used_data(lzws_decompressor_state_t* state_ptr);
void lzws_decompressor_clear_state(lzws_decompressor_state_t* state_ptr);
void lzws_decompressor_free_state(lzws_decompressor_state_t* state_ptr);

inline bool lzws_decompressor_is_dictionary_full(const lzws_decompressor_state_t* state_ptr)
{
  return state_ptr->free_code == LZWS_UNDEFINED_FREE_CODE;
}

inline size_t lzws_decompressor_get_total_codes_length(const lzws_decompressor_state_t* state_ptr)
{
  return state_ptr->max_code + 1;
}

#endif // LZWS_DECOMPRESSOR_STATE_H
