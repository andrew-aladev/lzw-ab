#!/bin/bash
set -e

DIR=$(dirname "${BASH_SOURCE[0]}")
source "${DIR}/../env.sh"

FROM_IMAGE="docker.io/gentoo/stage3-amd64-nomultilib"
IMAGE_NAME="${IMAGE_PREFIX}_toolchains"
IMAGE_PLATFORM="linux/amd64"

TARGETS_ARRAY=(
  "x86_64-unknown-linux-gnu"
  "i686-unknown-linux-gnu"

  "x86_64-gentoo-linux-musl"
  "i686-gentoo-linux-musl"

  "aarch64-unknown-linux-gnu"
  "aarch64_be-unknown-linux-gnu"

  "aarch64-gentoo-linux-musl"
  "aarch64_be-gentoo-linux-musl"
)

TARGETS="${TARGETS_ARRAY[@]}"
