#!/usr/bin/env bash
set -e

DIR=$(dirname "${BASH_SOURCE[0]}")
cd "$DIR"

CPU_COUNT=$(grep -c "^processor" "/proc/cpuinfo" || sysctl -n "hw.ncpu")

TMP_PATH="$(pwd)/../tmp"
TMP_SIZE="64"

./temp/mount.sh "$TMP_PATH" "$TMP_SIZE"
cd "$TMP_PATH"

build="toolchain-build"
mkdir -p "$build"
cd "$build"

if ([ -n "$CI" ]); then
  curl -s "https://codecov.io/bash" > "codecov.sh"
  chmod +x "codecov.sh"
fi

# We need to make test builds for all possible toolchains and dictionaries.

DICTIONARIES=("linked-list" "sparse-array")
BIGNUM_LIBRARIES=("gmp" "tommath")

toolchains="../../cmake/toolchains"
some_test_passed=false

while read -r toolchain; do
  for dictionary in "${DICTIONARIES[@]}"; do
    for bignum_library in "${BIGNUM_LIBRARIES[@]}"; do
      echo "toolchain: ${toolchain}, dictionary: ${dictionary}, bignum library: ${bignum_library}"

      # Only special toolchain can use coverage.
      if (echo "$toolchain" | grep -q "coverage.cmake$"); then
        COVERAGE_TOOLCHAIN=true
        if ! ([ -n "$CI" ]); then
          continue
        fi
      else
        COVERAGE_TOOLCHAIN=false
      fi

      find "." -depth \( \
        -name "CMake*" \
        -o -name "*.cmake" \
      \) -exec rm -rf {} +

      if [ "$COVERAGE_TOOLCHAIN" = true ]; then
        find "." -depth \( \
          -name "*.gcov" \
          -o -name "*.gcda" \
          -o -name "*.gcno" \
        \) -exec rm -rf {} +
      fi

      coverage=$(if [ "$COVERAGE_TOOLCHAIN" = true ]; then echo "ON"; else echo "OFF"; fi)
      build_type=$(if [ "$COVERAGE_TOOLCHAIN" = true ]; then echo "Debug"; else echo "Release"; fi)

      # Toolchain may not work.
      cmake "../.." \
        -DCMAKE_TOOLCHAIN_FILE="$toolchain" \
        -DCMAKE_GENERATOR_USE_CURRENT_TOOLCHAIN=ON \
        -DLZWS_COMPRESSOR_DICTIONARY="$dictionary" \
        -DLZWS_BIGNUM_LIBRARY="$bignum_library" \
        -DLZWS_SHARED=ON \
        -DLZWS_STATIC=ON \
        -DLZWS_CLI=OFF \
        -DLZWS_TESTS=ON \
        -DLZWS_EXAMPLES=ON \
        -DLZWS_MAN=OFF \
        -DLZWS_COVERAGE=$coverage \
        -DCMAKE_BUILD_TYPE="$build_type" \
        || continue

      cmake --build "." --target "clean"
      cmake --build "." -j${CPU_COUNT} --target "check" --config "$build_type"

      if ([ "$COVERAGE_TOOLCHAIN" = true ] && [ -n "$CI" ]); then
        if (echo "$toolchain" | grep -q "clang/coverage.cmake$") && (command -v "llvm-cov" > /dev/null 2>&1); then
          cov="llvm-cov gcov"
        else
          cov="gcov"
        fi

        ./codecov.sh -x "$cov" || continue
      fi

      some_test_passed=true
    done
  done
done < <(find "$toolchains" -type f)

if [ "$some_test_passed" = false ]; then
  >&2 echo "At least one test should pass"
  exit 1
fi
