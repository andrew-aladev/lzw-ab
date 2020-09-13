#!/bin/bash
set -e

DIR=$(dirname "${BASH_SOURCE[0]}")
cd "$DIR"

./0-toolchain/run.sh
./1-main/run.sh
