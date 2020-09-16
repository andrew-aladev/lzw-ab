#!/bin/bash
set -e

DIR=$(dirname "${BASH_SOURCE[0]}")
cd "$DIR"

TMP_PATH="$(pwd)/../../../../tmp"
TMP_SIZE="64"

../../../temp/mount.sh "$TMP_PATH" "$TMP_SIZE"
DATA_OPTIONS="--volume ${TMP_PATH}:/mnt/data"

CONTAINER_OPTIONS="$DATA_OPTIONS" ./0-toolchain/run.sh
CONTAINER_OPTIONS="$DATA_OPTIONS" ./1-main/run.sh
