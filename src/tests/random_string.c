// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "random_string.h"

#include <time.h>

LZWS_EXPORT void lzws_tests_set_random_string(char* buffer, size_t buffer_length)
{
  if (buffer_length == 0) {
    return;
  }

  size_t   last_index = buffer_length - 1;
  uint32_t value      = time(NULL);

  for (size_t index = 0; index < last_index; index++) {
    value         = value * 1103515245 + 12345;
    buffer[index] = ((value / 65536) % 255) + 1;
  }

  buffer[last_index] = '\0';
}
