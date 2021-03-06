#!/usr/bin/env bash
set -e

DIR=$(dirname "${BASH_SOURCE[0]}")
cd "$DIR"

./format-config/update.sh

BASE_DIR="../.."
SOURCES=(
  "${BASE_DIR}/cmake"
  "${BASE_DIR}/src"
)

find ${SOURCES[@]} \( -name "*.h" -o -name "*.c" \) -exec clang-format -i {} \;
