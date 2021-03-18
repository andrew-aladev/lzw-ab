// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// clang-format off
#define log_format(x) _Generic((x), \
  bool: "%d", \
  char *: "%s", \
  float: "%f", \
  size_t: "%zu", \
  void *: "%p" \
)
// clang-format on

#define log(x) printf(log_format(x), x), putchar('\n');

void func(void)
{
  log(u8"fit");

  static_assert(1 < 2, "1 < 2");

  exit(0);
}

int main(void)
{
  func();

  return 0;
}
