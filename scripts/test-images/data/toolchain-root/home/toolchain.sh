#!/bin/bash
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

for dictionary in "linked-list" "sparse-array"; do
  echo "dictionary: ${dictionary}"

  # Cleanup.
  rm -rf "$dictionary"

  mkdir "$dictionary"
  cd "$dictionary"

  TARGET="$TARGET" cmake "../.." \
    -DCMAKE_TOOLCHAIN_FILE="/home/toolchain.cmake" \
    -DLZWS_COMPRESSOR_DICTIONARY="$dictionary" \
    -DLZWS_SHARED=ON \
    -DLZWS_STATIC=ON \
    -DLZWS_TESTS=ON \
    -DLZWS_EXAMPLES=ON \
    -DLZWS_MAN=OFF \
    -DCMAKE_BUILD_TYPE="RELEASE"

  make clean
  make -j${CPU_COUNT}

  cd ".."
done
