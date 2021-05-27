#!/usr/bin/env bash
set -e

DIR=$(dirname "${BASH_SOURCE[0]}")
source "${DIR}/../env.sh"

FROM_IMAGE="${DOCKER_HOST}/${DOCKER_USERNAME}/test_${TARGET}_toolchain"

IMAGE_BUILD_ARGS="FROM_IMAGE TARGET"
IMAGE_NAME="${IMAGE_PREFIX}_${TARGET}_toolchain"
IMAGE_PLATFORM="linux/amd64"
IMAGE_LAYERS="false"
