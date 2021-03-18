// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include <assert.h>
#include <gmp.h>

// We are going to create numbers, add something and compare results.

int main(void)
{
  mpz_t a, b;

  // a = 0.
  mpz_init(a);

  // b = 0.
  mpz_init(b);

  // a = a + 1.
  mpz_add_ui(a, a, 1);

  // b = b + 2.
  mpz_add_ui(b, b, 2);

  // a (1) < b (2).
  assert(mpz_cmp(a, b) < 0);

  // a = a (1) * 3.
  mpz_mul_ui(a, a, 3);

  // b = b (2) * 1.
  mpz_mul_ui(b, b, 1);

  // b (2) < a (3).
  assert(mpz_cmp(b, a) < 0);

  mpz_clear(a);
  mpz_clear(b);

  return 0;
}
