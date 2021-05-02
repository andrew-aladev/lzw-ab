#!/usr/bin/env bash
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

    # Releases with GMP provided without static linked binaries (GMP LGPL license limitation).
    static=$(if [ "$bignum_library" = "gmp" ]; then echo "OFF"; else echo "ON"; fi)

    # It may not work on target platform.
    cmake ".." \
      -DLZWS_COMPRESSOR_DICTIONARY="$dictionary" \
      -DLZWS_BIGNUM_LIBRARY="$bignum_library" \
      -DLZWS_SHARED=ON \
      -DLZWS_STATIC=$static \
      -DLZWS_CLI=ON \
      -DLZWS_TESTS=ON \
      -DLZWS_COVERAGE=OFF \
      -DLZWS_EXAMPLES=ON \
      -DLZWS_MAN=ON \
      -DCMAKE_BUILD_TYPE="Release" \
      || continue

    cmake --build "." --target "clean"
    cmake --build "." -j${CPU_COUNT} --target "check" --config "Release" --verbose
    cmake --build "." --target "package" --config "Release"

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
  "gzip $COMPRESSION_LEVEL"
  "zip $COMPRESSION_LEVEL"
)
TAR_EXTENSIONS=(
  "tar.gz"
  "zip"
)
CURRENT_BRANCH="$(git branch --show-current)"

for index in ${!TAR_COMMANDS[@]}; do
  git archive "$CURRENT_BRANCH" --format="tar" | \
    ${TAR_COMMANDS[$index]} > "build/${NAME}.${TAR_EXTENSIONS[$index]}"
done

git archive "$CURRENT_BRANCH" --format="zip" $COMPRESSION_LEVEL -o "build/${NAME}.zip"
