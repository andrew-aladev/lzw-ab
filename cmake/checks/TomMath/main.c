// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include <assert.h>
#include <tommath.h>

// We are going to create numbers, add something and compare results.

int main()
{
  mp_int a, b, temp;

  // a = 0.
  assert(mp_init(&a) == MP_OKAY);

  // b = 0.
  assert(mp_init(&b) == MP_OKAY);

  // temp = 0.
  assert(mp_init(&temp) == MP_OKAY);

  // a = a + 1.
  mp_set_u32(&temp, 1);
  assert(mp_add(&a, &temp, &a) == MP_OKAY);

  // b = b + 2.
  mp_set_u32(&temp, 2);
  assert(mp_add(&b, &temp, &b) == MP_OKAY);

  // a (1) < b (2).
  assert(mp_cmp(&a, &b) == MP_LT);

  // a = a (1) * 3.
  mp_set_u32(&temp, 3);
  assert(mp_mul(&a, &temp, &a) == MP_OKAY);

  // b = b (2) * 1.
  mp_set_u32(&temp, 1);
  assert(mp_mul(&b, &temp, &b) == MP_OKAY);

  // b (2) < a (3).
  assert(mp_cmp(&b, &a) == MP_LT);

  mp_clear(&a);
  mp_clear(&b);
  mp_clear(&temp);

  return 0;
}
