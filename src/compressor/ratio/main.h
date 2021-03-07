// LZW streaming compressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#if !defined(LZWS_COMPRESSOR_RATIO_MAIN_H)
#define LZWS_COMPRESSOR_RATIO_MAIN_H

#include "common.h"

lzws_result_t lzws_compressor_initialize_ratio(lzws_compressor_ratio_t* ratio_ptr, bool quiet);

inline void lzws_compressor_add_source_symbol_to_ratio(lzws_compressor_ratio_t* ratio_ptr)
{
  ratio_ptr->new_source_length++;
}

inline void lzws_compressor_add_destination_symbol_to_ratio(lzws_compressor_ratio_t* ratio_ptr)
{
  ratio_ptr->new_destination_length++;
}

lzws_result_t lzws_compressor_need_to_clear_by_ratio(lzws_compressor_ratio_t* ratio_ptr, bool quiet, bool* result_ptr);
lzws_result_t lzws_compressor_clear_ratio(lzws_compressor_ratio_t* ratio_ptr, bool quiet);

inline void lzws_compressor_free_ratio(lzws_compressor_ratio_t* ratio_ptr)
{
  lzws_bigint_clear_multiple(&ratio_ptr->source_length, &ratio_ptr->destination_length, NULL);
}

#endif // LZWS_COMPRESSOR_RATIO_MAIN_H
