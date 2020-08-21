#!/bin/bash
set -e

DIR=$(dirname "${BASH_SOURCE[0]}")
cd "$DIR"

env-update
source "/etc/profile"

git clone "https://github.com/andrew-aladev/lzws.git" \
  --single-branch \
  --branch "master" \
  --depth 1 \
  "lzws"

cd "lzws"
./scripts/toolchains.sh
