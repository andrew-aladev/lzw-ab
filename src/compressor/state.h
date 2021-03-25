// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_COMPRESSOR_STATE_H)
#define LZWS_COMPRESSOR_STATE_H

#include "alignment/common.h"
#include "common.h"
#include "dictionary/common.h"
#include "ratio/common.h"

#if defined(__cplusplus)
extern "C" {
#endif

enum
{
  LZWS_COMPRESSOR_WRITE_MAGIC_HEADER = 1,
  LZWS_COMPRESSOR_WRITE_HEADER,
  LZWS_COMPRESSOR_ALLOCATE_DICTIONARY,
  LZWS_COMPRESSOR_READ_FIRST_SYMBOL,
  LZWS_COMPRESSOR_READ_NEXT_SYMBOL,
  LZWS_COMPRESSOR_WRITE_CURRENT_CODE,
  LZWS_COMPRESSOR_FLUSH_CURRENT_CODE,
  LZWS_COMPRESSOR_WRITE_REMAINDER_BEFORE_READ_NEXT_SYMBOL,
  LZWS_COMPRESSOR_WRITE_REMAINDER_BEFORE_CURRENT_CODE,
  LZWS_COMPRESSOR_FLUSH_REMAINDER_BEFORE_CURRENT_CODE,
  LZWS_COMPRESSOR_FLUSH_REMAINDER,
  LZWS_COMPRESSOR_WRITE_ALIGNMENT_BEFORE_READ_NEXT_SYMBOL,
  LZWS_COMPRESSOR_WRITE_ALIGNMENT_BEFORE_CURRENT_CODE,
  LZWS_COMPRESSOR_FLUSH_ALIGNMENT_BEFORE_CURRENT_CODE
};
typedef lzws_byte_fast_t lzws_compressor_status_t;

typedef struct
{
  lzws_compressor_options_t options;
  lzws_compressor_status_t  status;

  lzws_code_fast_t first_free_code;
  lzws_code_fast_t max_code;

  lzws_code_fast_t last_used_code;
  lzws_byte_fast_t last_used_code_bit_length;
  lzws_code_fast_t max_last_used_code_for_bit_length;

  lzws_code_fast_t current_code;
  lzws_byte_fast_t next_symbol;

  lzws_byte_fast_t remainder;
  lzws_byte_fast_t remainder_bit_length;

  lzws_compressor_alignment_t  alignment;
  lzws_compressor_dictionary_t dictionary;
  lzws_compressor_ratio_t      ratio;
} lzws_compressor_state_t;

LZWS_EXPORT lzws_result_t
  lzws_compressor_get_initial_state(lzws_compressor_state_t** state_ptr, const lzws_compressor_options_t* options_ptr);

LZWS_EXPORT void          lzws_compressor_reset_last_used_data(lzws_compressor_state_t* state_ptr);
LZWS_EXPORT lzws_result_t lzws_compressor_clear_state(lzws_compressor_state_t* state_ptr);
LZWS_EXPORT void          lzws_compressor_free_state(lzws_compressor_state_t* state_ptr);

inline bool lzws_compressor_is_dictionary_full(const lzws_compressor_state_t* state_ptr)
{
  return state_ptr->last_used_code == state_ptr->max_code;
}

inline size_t lzws_compressor_get_last_used_codes_length(const lzws_compressor_state_t* state_ptr)
{
  return state_ptr->last_used_code + 1;
}

inline size_t lzws_compressor_get_total_codes_length(const lzws_compressor_state_t* state_ptr)
{
  return state_ptr->max_code + 1;
}

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // LZWS_COMPRESSOR_STATE_H
