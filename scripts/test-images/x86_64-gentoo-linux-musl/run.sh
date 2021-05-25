#!/usr/bin/env bash
set -e

DIR=$(dirname "${BASH_SOURCE[0]}")
cd "$DIR"

CONTAINER_OPTIONS="--volume $(pwd)/../../..:/mnt/data"

CONTAINER_OPTIONS="$CONTAINER_OPTIONS" ./0-toolchain/run.sh
CONTAINER_OPTIONS="$CONTAINER_OPTIONS" ./1-main/run.sh
