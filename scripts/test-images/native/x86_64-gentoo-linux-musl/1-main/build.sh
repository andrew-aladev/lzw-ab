#!/usr/bin/env bash
set -e

DIR=$(dirname "${BASH_SOURCE[0]}")
cd "$DIR"

source "../../../utils.sh"
source "./env.sh"

fusermount -zu attached-main-root || :
bindfs -r -o nonempty "../../../data/main-root" attached-main-root
build "FROM_IMAGE" || error=$?
fusermount -zu attached-main-root || :

if [ ! -z "$error" ]; then
  exit "$error"
fi
