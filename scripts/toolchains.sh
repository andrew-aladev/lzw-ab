#!/bin/bash
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
toolchains="../../cmake/toolchains"

some_test_passed=false

while read -r toolchain; do
  for dictionary in "linked-list" "sparse-array"; do
    echo "toolchain: ${toolchain}, dictionary: ${dictionary}"

    # Only special toolchain can use coverage.
    if (echo "$toolchain" | grep -q "coverage.cmake$"); then
      COVERAGE_TOOLCHAIN=true
      if ! ([ -n "$CI" ]); then
        continue
      fi
    else
      COVERAGE_TOOLCHAIN=false
    fi

    find . -depth \( \
      -name "CMake*" \
      -o -name "*.cmake" \
    \) -exec rm -rf {} +

    if [ "$COVERAGE_TOOLCHAIN" = true ]; then
      find . -depth \( \
        -name "*.gcov" \
        -o -name "*.gcda" \
        -o -name "*.gcno" \
      \) -exec rm -rf {} +
    fi

    # Toolchain may not work on target platform.
    cmake "../.." \
      -DCMAKE_TOOLCHAIN_FILE="$toolchain" \
      -DCMAKE_GENERATOR_USE_CURRENT_TOOLCHAIN=ON \
      -DLZWS_COMPRESSOR_DICTIONARY="$dictionary" \
      -DLZWS_SHARED=ON \
      -DLZWS_STATIC=ON \
      -DLZWS_CLI=OFF \
      -DLZWS_TESTS=ON \
      -DLZWS_COVERAGE=$(if [ "$COVERAGE_TOOLCHAIN" = true ]; then echo "ON"; else echo "OFF"; fi) \
      -DLZWS_EXAMPLES=ON \
      -DLZWS_MAN=OFF \
      -DCMAKE_BUILD_TYPE=$(if [ "$COVERAGE_TOOLCHAIN" = true ]; then echo "DEBUG"; else echo "RELEASE"; fi) \
      || continue

    make clean
    make -j${CPU_COUNT}
    CTEST_OUTPUT_ON_FAILURE=1 make test

    if ([ "$COVERAGE_TOOLCHAIN" = true ] && [ -n "$CI" ]); then
      if (echo "$toolchain" | grep -q "clang/coverage.cmake$") && (command -v "llvm-cov" > /dev/null 2>&1); then
        ./codecov.sh -x "llvm-cov gcov" || continue
      else
        ./codecov.sh -x "gcov" || continue
      fi
    fi

    some_test_passed=true
  done
done < <(find "$toolchains" -type f)

if [ "$some_test_passed" = false ]; then
  echo "At least one test should pass" > "/dev/stderr"
  exit 1
fi
