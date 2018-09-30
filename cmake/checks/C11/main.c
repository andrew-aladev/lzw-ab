#include <assert.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <uchar.h>

// clang-format off
#define log_format(x) _Generic((x), \
  char *: "%s", \
  size_t: "%zu", \
  void *: "%p" \
)
// clang-format on

#define log(x) printf(log_format(x), x), putchar('\n');

typedef struct {
  bool is_float;
  union {
    float f;
    char *s;
  };
} data_t;

noreturn void func() {
  log(alignof(double));
  log(u8"fit");

  data_t data;
  data.f = 1.5;
  log((void *)data.s);

  static_assert(1 < 2, "1 < 2");

  exit(0);
}

int main() {
  func();
}