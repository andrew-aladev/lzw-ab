// LZW streaming decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "main.h"

#include "../log.h"
#include "alignment/wrapper.h"
#include "current_code.h"
#include "dictionary/wrapper.h"
#include "header.h"
#include "symbol.h"

// Decompressor will not expose LZWS_DECOMPRESSOR_NEEDS_MORE_SOURCE.
// Magic header and header are not required for empty string decompressing.

#define RETURN_FAILED_RESULT() \
  if (result != 0) {           \
    return result;             \
  }

#define RETURN_FAILED_RESULT_EXCLUDING_MORE_SOURCE()     \
  if (result != 0) {                                     \
    if (result == LZWS_DECOMPRESSOR_NEEDS_MORE_SOURCE) { \
      return 0;                                          \
    }                                                    \
                                                         \
    return result;                                       \
  }

LZWS_EXPORT lzws_result_t lzws_decompress(
  lzws_decompressor_state_t* state_ptr,
  lzws_byte_t**              source_ptr,
  size_t*                    source_length_ptr,
  lzws_byte_t**              destination_ptr,
  size_t*                    destination_length_ptr)
{
  lzws_result_t result;

  if (state_ptr->status == LZWS_DECOMPRESSOR_READ_MAGIC_HEADER) {
    result = lzws_decompressor_read_magic_header(state_ptr, source_ptr, source_length_ptr);
    RETURN_FAILED_RESULT_EXCLUDING_MORE_SOURCE();
  }

  if (state_ptr->status == LZWS_DECOMPRESSOR_READ_HEADER) {
    result = lzws_decompressor_read_header(state_ptr, source_ptr, source_length_ptr);
    RETURN_FAILED_RESULT_EXCLUDING_MORE_SOURCE();
  }

  if (state_ptr->status == LZWS_DECOMPRESSOR_ALLOCATE_DICTIONARY) {
    result = lzws_decompressor_allocate_dictionary_wrapper(state_ptr);
    RETURN_FAILED_RESULT();
  }

  lzws_decompressor_status_t status;

  while (true) {
    status = state_ptr->status;

    switch (status) {
      case LZWS_DECOMPRESSOR_READ_FIRST_CODE:
        result = lzws_decompressor_read_first_code(state_ptr, source_ptr, source_length_ptr);
        RETURN_FAILED_RESULT_EXCLUDING_MORE_SOURCE();
        break;

      case LZWS_DECOMPRESSOR_READ_NEXT_CODE:
        result = lzws_decompressor_read_next_code(state_ptr, source_ptr, source_length_ptr);
        RETURN_FAILED_RESULT_EXCLUDING_MORE_SOURCE();
        break;

      case LZWS_DECOMPRESSOR_WRITE_FIRST_SYMBOL:
        result = lzws_decompressor_write_first_symbol(state_ptr, destination_ptr, destination_length_ptr);
        RETURN_FAILED_RESULT();
        break;

      case LZWS_DECOMPRESSOR_WRITE_SYMBOLS_FOR_CURRENT_CODE:
        result = lzws_decompressor_write_symbols_for_current_code(state_ptr, destination_ptr, destination_length_ptr);
        RETURN_FAILED_RESULT();
        break;

      case LZWS_DECOMPRESSOR_READ_ALIGNMENT_BEFORE_FIRST_CODE:
        result = lzws_decompressor_read_alignment_before_first_code(state_ptr, source_ptr, source_length_ptr);
        RETURN_FAILED_RESULT_EXCLUDING_MORE_SOURCE();
        break;

      case LZWS_DECOMPRESSOR_READ_ALIGNMENT_BEFORE_NEXT_CODE:
        result = lzws_decompressor_read_alignment_before_next_code(state_ptr, source_ptr, source_length_ptr);
        RETURN_FAILED_RESULT_EXCLUDING_MORE_SOURCE();
        break;

      default:
        if (!state_ptr->options.quiet) {
          LZWS_LOG_ERROR("unknown status: %u", status);
        }

        return LZWS_DECOMPRESSOR_UNKNOWN_STATUS;
    }
  }

  return 0;
}
