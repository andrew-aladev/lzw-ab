// HTTP old generation (v0.9, v1.0, v1.1) C library.
// Copyright (c) 2019 AUTHORS, MIT License.

#if !defined(PRINT_DATA_H)
#define PRINT_DATA_H

#include <stdbool.h>

#include "print.h"

#define TERMINATOR ",\n"

#define INITIALIZE_SPACERS() \
  bool is_first_spacer = true;

#define PRINT_SPACER()       \
  if (is_first_spacer) {     \
    is_first_spacer = false; \
  }                          \
  else {                     \
    PRINT(TERMINATOR);       \
  }

#endif // PRINT_DATA_H
