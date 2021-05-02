#!/usr/bin/env bash
set -e

DIR=$(dirname "${BASH_SOURCE[0]}")
cd "$DIR"

SOURCE_PATH=$(realpath "../../../..")
VOLUME_OPTIONS="--volume ${SOURCE_PATH}:/mnt/data"

CONTAINER_OPTIONS="$VOLUME_OPTIONS" ./0-toolchain/run.sh
CONTAINER_OPTIONS="$VOLUME_OPTIONS" ./1-main/run.sh
