#!/bin/bash
set -e

DIR=$(dirname "${BASH_SOURCE[0]}")
cd "$DIR"

CPU_COUNT=$(grep -c "^processor" "/proc/cpuinfo" || sysctl -n "hw.ncpu")

cd "../build"

# Packing binaries.

DICTIONARIES=("linked-list" "sparse-array")
BIGNUM_LIBRARIES=("gmp" "tommath")

for dictionary in "${DICTIONARIES[@]}"; do
  for bignum_library in "${BIGNUM_LIBRARIES[@]}"; do
    echo "dictionary: ${dictionary}, bignum library: ${bignum_library}"
    build_dir="${dictionary}-${bignum_library}"

    # Cleanup.
    rm -rf "$build_dir"

    mkdir "$build_dir"
    cd "$build_dir"

    cmake "../.." \
      -G "Unix Makefiles" \
      -DLZWS_COMPRESSOR_DICTIONARY="$dictionary" \
      -DLZWS_BIGNUM_LIBRARY="$bignum_library" \
      -DCMAKE_BUILD_TYPE="RELEASE"
    make clean
    make -j${CPU_COUNT} VERBOSE=1

    CTEST_OUTPUT_ON_FAILURE=1 make test

    make package

    cd ".."
  done
done

cd ".."

# Packing source.

VERSION=$(grep "LZWS_VERSION" "CMakeLists.txt" | sed "s/.*LZWS_VERSION\s*['\"]\([0-9.]*\).*/\1/g")
NAME="lzws-${VERSION}"

COMPRESSION_LEVEL="-9"
TAR_COMMANDS=(
  "bzip2 $COMPRESSION_LEVEL"
  "gzip $COMPRESSION_LEVEL"
  "xz $COMPRESSION_LEVEL"
  "zip $COMPRESSION_LEVEL"
)
TAR_EXTENSIONS=(
  "tar.bz2"
  "tar.gz"
  "tar.xz"
  "zip"
)
CURRENT_BRANCH="$(git branch --show-current)"

for index in ${!TAR_COMMANDS[@]}; do
  git archive "$CURRENT_BRANCH" --format="tar" | \
    ${TAR_COMMANDS[$index]} > "build/${NAME}.${TAR_EXTENSIONS[$index]}"
done

git archive "$CURRENT_BRANCH" --format="zip" $COMPRESSION_LEVEL -o "build/${NAME}.zip"
