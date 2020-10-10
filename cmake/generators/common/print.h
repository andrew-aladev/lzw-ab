// HTTP old generation (v0.9, v1.0, v1.1) C library.
// Copyright (c) 2019 AUTHORS, MIT License.

#if !defined(PRINT_H)
#define PRINT_H

#include <stdbool.h>
#include <stdio.h>

#define PRINT(string) fputs(string, stdout)

#define PRINTF(...) fprintf(stdout, __VA_ARGS__);

// Spacers

#define TERMINATOR ",\n"

#define INITIALIZE_SPACERS(is_first) bool is_first_spacer = is_first;

#define PRINT_SPACER()       \
  if (is_first_spacer) {     \
    is_first_spacer = false; \
  } else {                   \
    PRINT(TERMINATOR);       \
  }

#endif // PRINT_H
