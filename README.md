# LZW streaming compressor/decompressor

| Travis | AppVeyor | Circle | Codecov |
| :---:  | :---:    | :---:  | :---:   |
| [![Travis test status](https://travis-ci.com/andrew-aladev/lzws.svg?branch=master)](https://travis-ci.com/andrew-aladev/lzws) | [![AppVeyor test status](https://ci.appveyor.com/api/projects/status/github/andrew-aladev/lzws?branch=master&svg=true)](https://ci.appveyor.com/project/andrew-aladev/lzws/branch/master) | [![Circle test status](https://circleci.com/gh/andrew-aladev/lzws/tree/master.svg?style=shield)](https://circleci.com/gh/andrew-aladev/lzws/tree/master) | [![Codecov](https://codecov.io/gh/andrew-aladev/lzws/branch/master/graph/badge.svg)](https://codecov.io/gh/andrew-aladev/lzws) |

LZW streaming compressor/decompressor based on LZW AB compatible with UNIX compress.
It has no legacy code from [ncompress](https://github.com/vapier/ncompress/blob/ncompress-4.2.4/compress42.c).

## Compressor dictionaries

- Linked list (idea from LZW AB). It has low memory usage <= 327 KB (16 bit codes). It is slow in general. It is recommended for small amount of data.
- Sparse array (enabled by default). It has high memory usage <= 33.5 MB (16 bit codes). It is fast. It is recommended for large amount of data.

You can add your own implementation.

## Why?

- You can support ancient software.
- You can make your application looks like ancient software.
- You can resurrect legacy apis (like `Content-Encoding: compress` in HTTP 1.1).

## New features

- Accurate ratio calculation without overhead, compressor provides smaller archive (compatible with UNIX compress).
- Unaligned bit groups switch (only disabled mode is compatible with UNIX compress).
- LSB/MSB switch (only LSB mode is compatible with UNIX compress).
- Magic header switch (only enabled magic header is compatible with UNIX compress).

## Dependencies

- Runtime dependency is [GMP](https://gmplib.org) only.
- Compilation dependencies: [cmake](https://cmake.org/), [asciidoc](http://asciidoc.org/) and [gcc](https://gcc.gnu.org/) or [clang](https://clang.llvm.org/).

## Operating systems

GNU/Linux, FreeBSD, OSX.

## Installation

You can download and install prebuilt versions [on releases page](https://github.com/andrew-aladev/lzws/releases).

```sh
sudo dpkg -i lzws-sparse-array-*.x86_64.deb
sudo rpm -i lzws-sparse-array-*.x86_64.rpm
```

You can find ebuilds for Gentoo in [scripts folder](scripts/gentoo).

On other systems you can unpack `lzws-sparse-array-*.x86_64.tar.gz` to `/usr/local` or build from source.

## Quick start

```sh
cd build
cmake .. && make
echo -n "TOBEORNOTTOBEORTOBEORNOT" | ./src/cli/lzws | ./src/cli/lzws -d
```

Debug build:
```sh
cmake .. -DLZWS_STATIC=ON -DLZWS_EXAMPLES=ON -DCMAKE_VERBOSE_MAKEFILE=ON
make clean && make VERBOSE=1
CTEST_OUTPUT_ON_FAILURE=1 make test
```

You can use different dictionaries:
```sh
cmake .. -DLZWS_COMPRESSOR_DICTIONARY="linked-list"/"sparse-array"
```

There is a script that tests all dictionaries using several toolchains (with sanitizers).
This script was used on CI.
```sh
../scripts/toolchains.sh
```

There is a script for release build.
```sh
../scripts/release.sh
```

You can test performance using linux kernel archive:
```sh
wget "https://cdn.kernel.org/pub/linux/kernel/v2.6/linux-2.6.39.4.tar.xz"
tar xf linux-2.6.39.4.tar.xz
tar cf linux.tar linux-2.6.39.4

cmake ".." \
  -DLZWS_COMPRESSOR_DICTIONARY="sparse-array" \
  -DCMAKE_BUILD_TYPE="RELEASE"
make clean && make

time ./src/cli/lzws < linux.tar > linux.tar.Z
time ./src/cli/lzws -d < linux.tar.Z > linux.tar.new
sha256sum linux.tar && sha256sum linux.tar.new
```

## Library quick start

There are string, file and generic APIs.
String and file APIs are very simple.

See the following example for string compression:
```c
const char text[]      = "example text";
size_t     text_length = strlen(text);

char*  compressed_text;
size_t compressed_text_length;

lzws_result_t result = lzws_compress_string(
  (lzws_byte_t*)text, text_length,
  (lzws_byte_t**)&compressed_text, &compressed_text_length, 0,
  false, LZWS_BIGGEST_MAX_CODE_BIT_LENGTH, true, false, false, false);

if (result != 0) {
  return 1;
}

free (compressed_text);
```

Generic API is framework agnostic and can be used to implement any binding with custom stream.
See [examples](src/examples) for more details.

You can build and test all examples:
```sh
cmake .. -DLZWS_EXAMPLES=ON
make clean && make
CTEST_OUTPUT_ON_FAILURE=1 make test
```

## CI

See universal test script [scripts/toolchains.sh](scripts/toolchains.sh) for CI.
Please visit [scripts/test-images](scripts/test-images).
You can run this test script using many native and cross images.

## Real world testing

See [lzws archive collector](https://github.com/andrew-aladev/lzws-archive-collector).

## Man

CLI:

- [lzws.1](man/lzws.1.txt)

String API:

- [lzws_compress_string.3](man/string/lzws_compress_string.3.txt)
- [lzws_decompress_string.3](man/string/lzws_decompress_string.3.txt)

File API:

- [lzws_compress_file.3](man/file/lzws_compress_file.3.txt)
- [lzws_decompress_file.3](man/file/lzws_decompress_file.3.txt)

Generic API:

- [lzws_create_source_buffer_for_compressor.3](man/generic/lzws_create_source_buffer_for_compressor.3.txt)
- [lzws_create_destination_buffer_for_compressor.3](man/generic/lzws_create_destination_buffer_for_compressor.3.txt)
- [lzws_create_source_buffer_for_decompressor.3](man/generic/lzws_create_source_buffer_for_decompressor.3.txt)
- [lzws_create_destination_buffer_for_decompressor.3](man/generic/lzws_create_destination_buffer_for_decompressor.3.txt)
- [lzws_resize_buffer.3](man/generic/lzws_resize_buffer.3.txt)
- [lzws_compressor_get_initial_state.3](man/generic/lzws_compressor_get_initial_state.3.txt)
- [lzws_compressor_free_state.3](man/generic/lzws_compressor_free_state.3.txt)
- [lzws_compress.3](man/generic/lzws_compress.3.txt)
- [lzws_compressor_finish.3](man/generic/lzws_compressor_finish.3.txt)
- [lzws_decompressor_get_initial_state.3](man/generic/lzws_decompressor_get_initial_state.3.txt)
- [lzws_decompressor_free_state.3](man/generic/lzws_decompressor_free_state.3.txt)
- [lzws_decompress.3](man/generic/lzws_decompress.3.txt)

## Documentation

- [compressor_ratio.txt](doc/compressor_ratio.txt)
- [compressor_with_linked_list.txt](doc/compressor_with_linked_list.txt)
- [compressor_with_sparse_array.txt](doc/compressor_with_sparse_array.txt)
- [decompressor.txt](doc/decompressor.txt)
- [output_compatibility.txt](doc/output_compatibility.txt)

## License

Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
Distributed under the BSD Software License (see LICENSE).

## Notes

Project depends on GMP with LGPL v2 license.
So it is not possible to distribute project binaries with statically linked GMP library.

Please use source code based distro like Gentoo if you want static linking.
End user can build, link and use any software in any mode (without distribution).
