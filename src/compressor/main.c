// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "main.h"

#include "../log.h"
#include "alignment/wrapper.h"
#include "common.h"
#include "current_code.h"
#include "dictionary/wrapper.h"
#include "header.h"
#include "remainder.h"
#include "symbol.h"

// Compressor will not expose LZWS_COMPRESSOR_NEEDS_MORE_SOURCE.
// Magic header and header are not required for empty string compressing.

#define RETURN_FAILED_RESULT() \
  if (result != 0) {           \
    return result;             \
  }

#define RETURN_FAILED_RESULT_EXCLUDING_MORE_SOURCE()   \
  if (result != 0) {                                   \
    if (result == LZWS_COMPRESSOR_NEEDS_MORE_SOURCE) { \
      return 0;                                        \
    }                                                  \
                                                       \
    return result;                                     \
  }

lzws_result_t lzws_compress(
  lzws_compressor_state_t* state_ptr,
  lzws_byte_t**            source_ptr,
  size_t*                  source_length_ptr,
  lzws_byte_t**            destination_ptr,
  size_t*                  destination_length_ptr)
{
  lzws_result_t result;

  if (state_ptr->status == LZWS_COMPRESSOR_WRITE_MAGIC_HEADER) {
    result = lzws_compressor_write_magic_header(state_ptr, destination_ptr, destination_length_ptr);
    RETURN_FAILED_RESULT();
  }

  if (state_ptr->status == LZWS_COMPRESSOR_WRITE_HEADER) {
    result = lzws_compressor_write_header(state_ptr, destination_ptr, destination_length_ptr);
    RETURN_FAILED_RESULT();
  }

  if (state_ptr->status == LZWS_COMPRESSOR_ALLOCATE_DICTIONARY) {
    result = lzws_compressor_allocate_dictionary_wrapper(state_ptr);
    RETURN_FAILED_RESULT();
  }

  if (state_ptr->status == LZWS_COMPRESSOR_READ_FIRST_SYMBOL) {
    result = lzws_compressor_read_first_symbol(state_ptr, source_ptr, source_length_ptr);
    RETURN_FAILED_RESULT_EXCLUDING_MORE_SOURCE();
  }

  lzws_compressor_status_t status;

  while (true) {
    status = state_ptr->status;

    switch (status) {
      case LZWS_COMPRESSOR_READ_NEXT_SYMBOL:
        result = lzws_compressor_read_next_symbol(state_ptr, source_ptr, source_length_ptr);
        RETURN_FAILED_RESULT_EXCLUDING_MORE_SOURCE();
        break;

      case LZWS_COMPRESSOR_WRITE_CURRENT_CODE:
        result = lzws_compressor_write_current_code(state_ptr, destination_ptr, destination_length_ptr);
        RETURN_FAILED_RESULT();
        break;

      case LZWS_COMPRESSOR_WRITE_REMAINDER_BEFORE_READ_NEXT_SYMBOL:
        result =
          lzws_compressor_write_remainder_before_read_next_symbol(state_ptr, destination_ptr, destination_length_ptr);
        RETURN_FAILED_RESULT();
        break;

      case LZWS_COMPRESSOR_WRITE_REMAINDER_BEFORE_CURRENT_CODE:
        result =
          lzws_compressor_write_remainder_before_current_code(state_ptr, destination_ptr, destination_length_ptr);
        RETURN_FAILED_RESULT();
        break;

      case LZWS_COMPRESSOR_WRITE_ALIGNMENT_BEFORE_READ_NEXT_SYMBOL:
        result =
          lzws_compressor_write_alignment_before_read_next_symbol(state_ptr, destination_ptr, destination_length_ptr);
        RETURN_FAILED_RESULT();
        break;

      case LZWS_COMPRESSOR_WRITE_ALIGNMENT_BEFORE_CURRENT_CODE:
        result =
          lzws_compressor_write_alignment_before_current_code(state_ptr, destination_ptr, destination_length_ptr);
        RETURN_FAILED_RESULT();
        break;

      default:
        if (!state_ptr->options.quiet) {
          LZWS_LOG_ERROR("unknown status: %u", status);
        }

        return LZWS_COMPRESSOR_UNKNOWN_STATUS;
    }
  }

  return 0;
}

lzws_result_t lzws_compressor_finish(
  lzws_compressor_state_t* state_ptr,
  lzws_byte_t**            destination_ptr,
  size_t*                  destination_length_ptr)
{
  switch (state_ptr->status) {
    case LZWS_COMPRESSOR_WRITE_MAGIC_HEADER:
    case LZWS_COMPRESSOR_WRITE_HEADER:
    case LZWS_COMPRESSOR_READ_FIRST_SYMBOL:
      // We have no current code and remainder yet.
      return 0;

    case LZWS_COMPRESSOR_READ_NEXT_SYMBOL:
      // We have current code and maybe remainder.
      lzws_compressor_process_eof_before_next_symbol(state_ptr);
      break;
  }

  lzws_result_t result;

  if (state_ptr->status == LZWS_COMPRESSOR_FLUSH_REMAINDER_BEFORE_CURRENT_CODE) {
    result = lzws_compressor_flush_remainder_before_current_code(state_ptr, destination_ptr, destination_length_ptr);
    RETURN_FAILED_RESULT();
  }

  if (state_ptr->status == LZWS_COMPRESSOR_FLUSH_ALIGNMENT_BEFORE_CURRENT_CODE) {
    result = lzws_compressor_flush_alignment_before_current_code(state_ptr, destination_ptr, destination_length_ptr);
    RETURN_FAILED_RESULT();
  }

  if (state_ptr->status == LZWS_COMPRESSOR_FLUSH_CURRENT_CODE) {
    result = lzws_compressor_flush_current_code(state_ptr, destination_ptr, destination_length_ptr);
    RETURN_FAILED_RESULT();
  }

  lzws_compressor_status_t status = state_ptr->status;

  switch (status) {
    case LZWS_COMPRESSOR_FLUSH_REMAINDER:
      return lzws_compressor_flush_remainder(state_ptr, destination_ptr, destination_length_ptr);

    default:
      if (!state_ptr->options.quiet) {
        LZWS_LOG_ERROR("unknown status: %u", status);
      }

      return LZWS_COMPRESSOR_UNKNOWN_STATUS;
  }
}
