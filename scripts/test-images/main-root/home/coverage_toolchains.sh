#!/usr/bin/env bash
set -e

env-update
source "/etc/profile"

DIR="/mnt/data"

if [ -d "$DIR" ]; then
  # Testing cross compiled build.
  DICTIONARIES=("linked-list" "sparse-array")
  BIGNUM_LIBRARIES=("gmp" "tommath")

  for dictionary in "${DICTIONARIES[@]}"; do
    for bignum_library in "${BIGNUM_LIBRARIES[@]}"; do
      echo "dictionary: ${dictionary}, bignum library: ${bignum_library}"
      cd "$DIR/build/${dictionary}_${bignum_library}"

      ctest
    done
  done

  cd "$DIR"
else
  mkdir -p "$DIR"
  cd "$DIR"

  git clone "https://github.com/andrew-aladev/lzws.git" \
    --single-branch \
    --branch "master" \
    --depth 1 \
    "."
fi

./scripts/coverage_toolchains.sh
