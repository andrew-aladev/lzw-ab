#!/bin/bash
set -e

DIR=$(dirname "${BASH_SOURCE[0]}")
cd "$DIR"

CPU_COUNT=$(grep -c "^processor" "/proc/cpuinfo" || sysctl -n "hw.ncpu")

cd "../build"

# Packing binaries.

find . \( -name "CMake*" -o -name "*.cmake" \) -depth -exec rm -rf {} +

for dictionary in "linked-list" "sparse-array"; do
  cmake ".." \
    -DLZWS_COMPRESSOR_DICTIONARY="$dictionary" \
    -DCMAKE_BUILD_TYPE="RELEASE"
  make clean
  make -j${CPU_COUNT} VERBOSE=1

  CTEST_OUTPUT_ON_FAILURE=1 make test

  make package
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
