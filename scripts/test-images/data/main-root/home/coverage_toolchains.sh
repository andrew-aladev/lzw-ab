#!/bin/bash
set -e

env-update
source "/etc/profile"

DIR="/mnt/data"

if [ -d "$DIR" ]; then
  # Testing cross compiled build.
  cd "$DIR/build"

  for dictionary in "linked-list" "sparse-array"; do
    echo "dictionary: ${dictionary}"

    cd "$dictionary"
    make test

    cd ".."
  done

  cd ".."
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
