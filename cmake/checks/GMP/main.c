// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include <assert.h>
#include <gmp.h>

// We are going to create numbers, add something and compare results.

int main()
{
  mpz_t a, b, c, r1, r2;
  mpz_inits(a, b, c, r1, r2, NULL);

  mpz_add_ui(a, a, 1);
  mpz_add_ui(b, b, 2);
  mpz_add_ui(c, c, 3);

  mpz_mul(r1, a, b);
  mpz_mul(r2, b, c);

  assert(mpz_cmp(r1, r2) < 0);

  mpz_clears(a, b, c, r1, r2, NULL);

  return 0;
}
