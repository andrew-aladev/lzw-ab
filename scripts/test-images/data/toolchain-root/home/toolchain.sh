#!/usr/bin/env bash
set -e

env-update
source "/etc/profile"
source "/usr/local/bin/target-env.sh"

CPU_COUNT=$(grep -c "^processor" "/proc/cpuinfo" || sysctl -n "hw.ncpu")

DIR="/mnt/data"

if [ ! -d "$DIR" ]; then
  mkdir -p "$DIR"

  git clone "https://github.com/andrew-aladev/lzws.git" \
    --single-branch \
    --branch "master" \
    --depth 1 \
    "$DIR"
fi

cd "$DIR/build"

some_test_passed=false

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
    TARGET="$TARGET" cmake ".." \
      -DCMAKE_TOOLCHAIN_FILE="/home/toolchain.cmake" \
      -DLZWS_COMPRESSOR_DICTIONARY="$dictionary" \
      -DLZWS_BIGNUM_LIBRARY="$bignum_library" \
      -DLZWS_SHARED=ON \
      -DLZWS_STATIC=ON \
      -DLZWS_CLI=OFF \
      -DLZWS_TESTS=ON \
      -DLZWS_COVERAGE=OFF \
      -DLZWS_EXAMPLES=ON \
      -DLZWS_MAN=OFF \
      -DCMAKE_BUILD_TYPE="Release" \
      || continue

    cmake --build "." --target "clean"
    cmake --build "." -j${CPU_COUNT} --config "Release"

    some_test_passed=true
  done
done

if [ "$some_test_passed" = false ]; then
  >&2 echo "At least one test should pass"
  exit 1
fi
