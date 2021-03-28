// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "code.h"
#include "data.h"

int main(void)
{
  if (lzws_test_valid_codes() != 0) {
    return 1;
  }

  if (lzws_test_valid_datas() != 0) {
    return 2;
  }

  return 0;
}
