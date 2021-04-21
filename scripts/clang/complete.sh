#!/bin/bash
set -e

DIR=$(dirname "${BASH_SOURCE[0]}")
cd "$DIR"

cd "../.."
echo "-I$(pwd)/argtable3/src" > ".clang_complete"
