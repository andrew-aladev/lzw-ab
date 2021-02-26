// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include <assert.h>
#include <tommath.h>

// We are going to create numbers, add something and compare results.

int main()
{
  mp_int a, b, c, r1, r2, temp;
  assert(mp_init_multi(&a, &b, &c, &r1, &r2, &temp, NULL) == MP_OKAY);

  mp_set_u32(&temp, 1);
  assert(mp_add(&a, &temp, &a) == MP_OKAY);

  mp_set_u32(&temp, 2);
  assert(mp_add(&b, &temp, &b) == MP_OKAY);

  mp_set_u32(&temp, 3);
  assert(mp_add(&c, &temp, &c) == MP_OKAY);

  assert(mp_mul(&a, &b, &r1) == MP_OKAY);
  assert(mp_mul(&b, &c, &r2) == MP_OKAY);

  assert(mp_cmp(&r1, &r2) == MP_LT);

  mp_clear_multi(&a, &b, &c, &r1, &r2, &temp, NULL);

  return 0;
}
