#!/bin/bash
set -e

DIR=$(dirname "${BASH_SOURCE[0]}")
cd "$DIR"

CPU_COUNT=$(grep -c "^processor" "/proc/cpuinfo" || sysctl -n "hw.ncpu")

cd "../build"

# Packing binaries.

some_release_passed=false

DICTIONARIES=("linked-list" "sparse-array")
BIGNUM_LIBRARIES=("gmp" "tommath")

for dictionary in "${DICTIONARIES[@]}"; do
  for bignum_library in "${BIGNUM_LIBRARIES[@]}"; do
    echo "dictionary: ${dictionary}, bignum library: ${bignum_library}"

    find . -depth \( \
      -name "CMake*" \
      -o -name "*.cmake" \
    \) -exec rm -rf {} +

    # It may not work on target platform.
    cmake ".." \
      -DLZWS_COMPRESSOR_DICTIONARY="$dictionary" \
      -DLZWS_BIGNUM_LIBRARY="$bignum_library" \
      -DLZWS_SHARED=ON \
      -DLZWS_STATIC=ON \
      -DCMAKE_BUILD_TYPE="RELEASE" \
      || continue

    cmake --build "." --target "clean"
    cmake --build "." -j${CPU_COUNT} --target "check" --verbose
    cmake --build "." --target "package"

    some_release_passed=true
  done
done

if [ "$some_release_passed" = false ]; then
  >&2 echo "At least one release should pass"
  exit 1
fi

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
