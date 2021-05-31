# LZW streaming compressor/decompressor

| AppVeyor | Circle | Github Actions | Codecov |
| :------: | :----: | :------------: | :-----: |
| [![AppVeyor test status](https://ci.appveyor.com/api/projects/status/github/andrew-aladev/lzws?branch=master&svg=true)](https://ci.appveyor.com/project/andrew-aladev/lzws/branch/master) | [![Circle test status](https://circleci.com/gh/andrew-aladev/lzws/tree/master.svg?style=shield)](https://circleci.com/gh/andrew-aladev/lzws/tree/master) | [![Github Actions test status](https://github.com/andrew-aladev/lzws/workflows/test/badge.svg?branch=master)](https://github.com/andrew-aladev/lzws/actions) | [![Codecov](https://codecov.io/gh/andrew-aladev/lzws/branch/master/graph/badge.svg)](https://codecov.io/gh/andrew-aladev/lzws) |

LZW streaming compressor/decompressor based on [LZW AB](https://github.com/dbry/lzw-ab) compatible with [UNIX compress](https://en.wikipedia.org/wiki/Compress).

## Compressor dictionaries

- Linked list (idea from [LZW AB](https://github.com/dbry/lzw-ab)). It has low memory usage <= 327 KB (16 bit codes). It is slow in general, recommended for small amount of data.
- Sparse array (enabled by default). It has high memory usage <= 33.5 MB (16 bit codes). It is fast, recommended for large amount of data.

You can add your own implementation, see api for [dictionary](src/compressor/dictionary).

## Why?

- You can compress and decompress `.Z` (`tar.Z`) archives.
- You can resurrect legacy apis (like `Content-Encoding: compress` in HTTP 1.1).
- You can support ancient software and make your application looks like ancient software.

## New features

- Accurate ratio calculation without overhead, compressor provides smaller archive (compatible with UNIX compress).
- Unaligned bit groups switch (only disabled mode is compatible with UNIX compress).
- [LSB/MSB](https://en.wikipedia.org/wiki/Bit_numbering) switch (only LSB mode is compatible with UNIX compress).
- [Magic header](https://en.wikipedia.org/wiki/List_of_file_signatures) switch (only enabled magic header is compatible with UNIX compress).

## Dependencies

Operating systems: GNU/Linux, OSX, FreeBSD, Windows (Visual Studio, MinGW, Cygwin).

- Runtime dependencies: [GMP](https://gmplib.org) or [TomMath](https://github.com/libtom/libtommath).
- Compilation dependencies: [cmake](https://cmake.org), [gcc](https://gcc.gnu.org) or [clang](https://clang.llvm.org) or [msvc](https://docs.microsoft.com/en-us/cpp/c-language/?view=msvc-160), [asciidoc](http://asciidoc.org), [docbook xml 4.5](https://tdg.docbook.org/tdg/4.5/appa.html).
- Additional development tools: [yq](https://github.com/kislyuk/yq).

## Installation

You can download and install prebuilt packages [on releases page](https://github.com/andrew-aladev/lzws/releases).

You can use [scripts/release.sh](scripts/release.sh), it will create package for your system from source.
You can also use [overlay](https://github.com/andrew-aladev/overlay) for gentoo.

Installation for Windows (Visual Studio) from source is not so trivial:
1. Install Visual Studio with components: msvc, sdk.
2. Install [chocolatey](https://chocolatey.org) with components: cmake, curl, git, python, xsltproc, docbook-bundle, zip.
3. Install [asciidoc](https://asciidoc.org/INSTALL.html).
4. Download [docbook-xml-4.5.zip](https://docbook.org/xml/4.5/docbook-xml-4.5.zip) and unpack it to `C:\ProgramData\chocolatey\lib\docbook-bundle\docbook-4.5`.
5. Edit `C:\ProgramData\chocolatey\lib\docbook-bundle\catalog.xml` and append `<nextCatalog catalog="./docbook-4.5/catalog.xml"/>`.
6. Launch `x64 Native Tools Command Promp` as admin.

```cmd
git clone "https://github.com/libtom/libtommath.git" --depth 1 && cd "libtommath"
nmake -f "makefile.msvc" install && cd ".."
set "XML_CATALOG_FILES=C:\ProgramData\chocolatey\lib\docbook-bundle\catalog.xml"
set "INCLUDE=C:\devel\include;%INCLUDE%" && set "LIB=C:\devel\lib;%LIB%" && set "PATH=C:\devel\bin;%PATH%"
cd build && bash "..\scripts\release.sh"
```

## Quick start

```sh
cd build
cmake -DLZWS_COMPRESSOR_DICTIONARY="sparse-array" ".." && cmake --build "."
echo -n "TOBEORNOTTOBEORTOBEORNOT" | ./src/cli/lzws | ./src/cli/lzws -d
```

| Option                       | Values                           | Default          | Description               |
|------------------------------|----------------------------------|------------------|---------------------------|
| `LZWS_COMPRESSOR_DICTIONARY` | `sparse-array`, `linked-list`    | `sparse-array`   | dictionary implementation |
| `LZWS_BIGNUM_LIBRARY`        | `gmp`, `tommath`, `""` (auto)    | `""` (auto)      | bignum library            |
| `LZWS_SHARED`                | `ON`, `OFF`                      | `ON`             | build shared binaries     |
| `LZWS_STATIC`                | `ON`, `OFF`                      | `OFF`            | build static binaries     |
| `LZWS_CLI`                   | `ON`, `OFF`                      | `ON`             | build cli                 |
| `LZWS_TESTS`                 | `ON`, `OFF`                      | `ON`             | build tests               |
| `LZWS_EXAMPLES`              | `ON`, `OFF`                      | `OFF`            | build examples            |
| `LZWS_MAN`                   | `ON`, `OFF`                      | `OFF`            | build man                 |
| `LZWS_ARGTABLE3_FALLBACK`    | `github`, `github-archive`       | `github-archive` | argtable3 fallback        |
|                              | `bitbucket`, `bitbucket-archive` |                  |                           |
|                              | `gitlab`, `gitlab-archive`       |                  |                           |
| `LZWS_COVERAGE`              | `ON`, `OFF`                      | `OFF`            | enable coverage           |

## Examples and docs

See [`examples`](src/examples), [`man`](man) and [`doc`](doc) folders.

## CI

Please visit [scripts/test-images](scripts/test-images).
See test script [scripts/toolchains.sh](scripts/toolchains.sh) and
test script [scripts/coverage_toolchains.sh](scripts/coverage_toolchains.sh) for CI.

## Real world testing

See [lzws archive collector](https://github.com/andrew-aladev/lzws-archive-collector).

## License

Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see [AUTHORS](AUTHORS)).
Distributed under the BSD Software License (see [LICENSE](LICENSE)).

## Notice

Releases with GMP provided without static linked binaries (GMP LGPL license limitation).
