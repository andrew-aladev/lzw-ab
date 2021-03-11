#!/bin/bash
set -e

DIR=$(dirname "${BASH_SOURCE[0]}")
cd "$DIR"

./config-format/update.sh

BASE_DIR="../.."
SOURCES=(
  "${BASE_DIR}/cmake"
  "${BASE_DIR}/src"
)

find ${SOURCES[@]} \( -name "*.h" -o -name "*.c" \) -exec clang-format -i {} \;