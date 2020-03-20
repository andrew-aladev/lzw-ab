// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define XSTR(x) STR(x)
#define STR(x) #x

#define MAX_INDEX 16
#define NUMBER_WIDTH 4

#define BYTE_LENGTH_PER_LINE 4
#define BYTE_LENGTH_GLUE ", "
#define LINE_PREFIX "  "
#define LINE_TERMINATOR ",\n"

#define PRINT(string) fputs(string, stdout)

int main()
{
  for (uint8_t index = 0; index <= MAX_INDEX; index++) {
    if (index == 0) {
      PRINT(LINE_PREFIX);
    }
    else if (index % BYTE_LENGTH_PER_LINE == 0) {
      PRINT(LINE_TERMINATOR);
      PRINT(LINE_PREFIX);
    }
    else {
      PRINT(BYTE_LENGTH_GLUE);
    }

    printf("0x%0" XSTR(NUMBER_WIDTH) "x", (1 << index) - 1);
  }

  return 0;
}
