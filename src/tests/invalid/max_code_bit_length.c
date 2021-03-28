// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "max_code_bit_length.h"

#include "../../compressor/state.h"
#include "../../log.h"

static const lzws_byte_t max_code_bit_lengths[] = {
  LZWS_LOWEST_MAX_CODE_BIT_LENGTH - 1,
  LZWS_BIGGEST_MAX_CODE_BIT_LENGTH + 1};
#define MAX_CODE_BIT_LENGTH sizeof(max_code_bit_lengths)

lzws_result_t lzws_test_invalid_max_code_bit_length(void)
{
  lzws_result_t result;

  for (size_t index = 0; index < MAX_CODE_BIT_LENGTH; index++) {
    lzws_compressor_state_t* state_ptr;

    lzws_compressor_options_t options = LZWS_COMPRESSOR_DEFAULT_OPTIONS;
    options.max_code_bit_length       = max_code_bit_lengths[index];

    result = lzws_compressor_get_initial_state(&state_ptr, &options);
    if (result != LZWS_COMPRESSOR_INVALID_MAX_CODE_BIT_LENGTH) {
      LZWS_LOG_ERROR("compressor get initial state should fail with invaid max code bit length");
      return 1;
    }
  }

  return 0;
}
