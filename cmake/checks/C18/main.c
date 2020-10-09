// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include <assert.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

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

typedef struct {
  bool is_float;
  union {
    float number;
    char *string;
  };
} data_t;

const data_t data = {.is_float = true, .number = 1.5};

noreturn void func()
{
  log(alignof(double));
  log(u8"fit");
  log((void *)&data);
  log(data.is_float);
  log(data.number);

  static_assert(1 < 2, "1 < 2");

  exit(0);
}

int main() { func(); }
