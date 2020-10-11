// LZW streaming decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "symbol.h"

#include "../utils.h"
#include "alignment/wrapper.h"
#include "dictionary/wrapper.h"
#include "remainder.h"

lzws_result_t lzws_decompressor_write_first_symbol(
  lzws_decompressor_state_t* state_ptr,
  lzws_byte_t**              destination_ptr,
  size_t*                    destination_length_ptr)
{
  if (*destination_length_ptr < 1) {
    return LZWS_DECOMPRESSOR_NEEDS_MORE_DESTINATION;
  }

  lzws_write_byte(state_ptr->prefix_code, destination_ptr, destination_length_ptr);

  state_ptr->status = LZWS_DECOMPRESSOR_READ_NEXT_CODE;

  return 0;
}

lzws_result_t lzws_decompressor_write_symbols_for_current_code(
  lzws_decompressor_state_t* state_ptr,
  lzws_byte_t**              destination_ptr,
  size_t*                    destination_length_ptr)
{
  lzws_byte_t symbol;

  while (lzws_decompressor_has_symbol_in_dictionary_wrapper(state_ptr)) {
    if (*destination_length_ptr < 1) {
      return LZWS_DECOMPRESSOR_NEEDS_MORE_DESTINATION;
    }

    symbol = lzws_decompressor_get_symbol_from_dictionary_wrapper(state_ptr);
    lzws_write_byte(symbol, destination_ptr, destination_length_ptr);
  }

  // We need to check whether we need to read alignment.
  if (lzws_decompressor_need_to_read_alignment_wrapper(state_ptr)) {
    // Remainder is a part of alignment, we need to clear it.
    lzws_decompressor_clear_remainder(state_ptr);

    state_ptr->status = LZWS_DECOMPRESSOR_READ_ALIGNMENT_BEFORE_NEXT_CODE;
  } else {
    state_ptr->status = LZWS_DECOMPRESSOR_READ_NEXT_CODE;
  }

  return 0;
}
