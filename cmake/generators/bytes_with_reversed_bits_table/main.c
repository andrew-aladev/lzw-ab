// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include <stdint.h>

#include "../common/print.h"

#define BYTE_TEMPLATE        "0x%02x"
#define BYTE_LENGTH_PER_LINE 16
#define BYTE_LENGTH_GLUE     ", "

static inline void print_bytes_with_reversed_bits(uint8_t byte)
{
  uint8_t byte_with_reversed_bits = 0;

  for (uint8_t bit_index = 0; bit_index < 8; bit_index++) {
    uint8_t bit_value = (byte >> bit_index) & 1;
    byte_with_reversed_bits |= bit_value << (7 - bit_index);
  }

  PRINTF(BYTE_TEMPLATE, byte_with_reversed_bits);
}

int main()
{
  INITIALIZE_SPACERS(true);

  uint8_t index = 0;

  while (true) {
    if (index % BYTE_LENGTH_PER_LINE == 0) {
      PRINT_SPACER();
    } else {
      PRINT(BYTE_LENGTH_GLUE);
    }

    print_bytes_with_reversed_bits(index);

    if (index == UINT8_MAX) {
      break;
    }
    index++;
  }

  return 0;
}
