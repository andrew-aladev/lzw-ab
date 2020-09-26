#!/bin/bash
set -e

DIR=$(dirname "${BASH_SOURCE[0]}")
cd "$DIR"

source "../../../utils.sh"
source "./env.sh"

fusermount -zu attached-toolchain-root || true
bindfs -r -o nonempty "../../../data/toolchain-root" attached-toolchain-root
build "FROM_IMAGE TARGET" || error=$?
fusermount -zu attached-toolchain-root || true

if [ ! -z "$error" ]; then
  exit "$error"
fi