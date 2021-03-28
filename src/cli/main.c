// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include <getopt.h>
#include <stdlib.h>

#include "../file.h"

static const char* help =
  "Overview: lzws cli tool\n"
  "\n"
  "Usage: lzws [-%s] [< stdin] [> stdout]\n"
  "\n"
  "Options:\n"
  "  --decompress (-d) - enable decompress mode (default mode is compress)\n"
  "  --without-magic-header (-w) - disable magic header (enabled by default)\n"
  "  --max-code-bit-length (-b) - set max code bit length (%u-%u) (default value is %u) [compressor only]\n"
  "  --raw (-r) - disable block mode (enabled by default) [compressor only]\n"
  "  --msb (-m) - enable most significant bit (default mode is least significant bit)\n"
  "  --unaligned-bit-groups (-u) - enable unaligned bit groups (disabled by default)\n"
  "  --quiet (-q) - disable error messages (enabled by default)\n"
  "  --help (-h) - print help\n"
  "  --version (v) - print version\n"
  "\n"
  "Compatibility:\n"
  "  Default options are fully compatible with UNIX compress.\n"
  "  Compressor only options (-b and -r) can be changed without loosing compatibility.\n";

static const char*         short_options  = "dwb:rmuqhv";
static const struct option long_options[] = {
  {"decompress", optional_argument, NULL, 'd'},
  {"without-magic-header", optional_argument, NULL, 'w'},
  {"max-code-bit-length", optional_argument, NULL, 'b'},
  {"raw", optional_argument, NULL, 'r'},
  {"msb", optional_argument, NULL, 'm'},
  {"unaligned-bit-groups", optional_argument, NULL, 'u'},
  {"quiet", optional_argument, NULL, 'q'},
  {"help", optional_argument, NULL, 'h'},
  {"version", optional_argument, NULL, 'v'},
  {NULL, 0, NULL, 0}};

static inline void print_version(void)
{
  fprintf(stderr, "%s\n", LZWS_VERSION);
}

static inline void print_help(void)
{
  fprintf(
    stderr,
    help,
    short_options,
    LZWS_LOWEST_MAX_CODE_BIT_LENGTH,
    LZWS_BIGGEST_MAX_CODE_BIT_LENGTH,
    LZWS_COMPRESSOR_DEFAULT_OPTIONS.max_code_bit_length);
}

int main(int argc, char** argv)
{
  // All option values (except max code bit length) have to be set using raw values (instead of defaults).
  // It is required to keep options switch behaviour transparent.

  bool             is_compressor        = true;
  bool             without_magic_header = false;
  lzws_byte_fast_t max_code_bit_length  = LZWS_COMPRESSOR_DEFAULT_OPTIONS.max_code_bit_length;
  bool             block_mode           = true;
  bool             msb                  = false;
  bool             unaligned_bit_groups = false;
  bool             quiet                = false;
  int              option;

  while ((option = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
    switch (option) {
      case 'b':
        max_code_bit_length = atoi(optarg);
        break;
      case 'd':
        is_compressor = false;
        break;
      case 'm':
        msb = true;
        break;
      case 'q':
        quiet = true;
        break;
      case 'r':
        block_mode = false;
        break;
      case 'u':
        unaligned_bit_groups = true;
        break;
      case 'w':
        without_magic_header = true;
        break;
      case 'v':
        print_version();
        return 0;
      default:
        print_help();
        return 0;
    }
  }

  lzws_result_t result;

  if (is_compressor) {
    lzws_compressor_options_t options = {
      .without_magic_header = without_magic_header,
      .max_code_bit_length  = max_code_bit_length,
      .block_mode           = block_mode,
      .msb                  = msb,
      .unaligned_bit_groups = unaligned_bit_groups,
      .quiet                = quiet};

    result = lzws_compress_file(stdin, 0, stdout, 0, &options);
    if (result != 0) {
      return 1;
    }
  } else {
    lzws_decompressor_options_t options = {
      .without_magic_header = without_magic_header,
      .msb                  = msb,
      .unaligned_bit_groups = unaligned_bit_groups,
      .quiet                = quiet};

    result = lzws_decompress_file(stdin, 0, stdout, 0, &options);
    if (result != 0) {
      return 2;
    }
  }

  return 0;
}
