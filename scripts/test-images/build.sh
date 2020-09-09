#!/bin/bash
set -e

DIR=$(dirname "${BASH_SOURCE[0]}")
cd "$DIR"

./native/build.sh
./cross/build.sh
./toolchains/build.sh
