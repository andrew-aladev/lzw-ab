#!/usr/bin/env bash
set -e

DIR=$(dirname "${BASH_SOURCE[0]}")
source "${DIR}/../env.sh"

FROM_IMAGE="${DOCKER_HOST}/${DOCKER_USERNAME}/test_${TARGET}_toolchain"

IMAGE_BUILD_ARGS="FROM_IMAGE TARGET"
IMAGE_NAME="${IMAGE_PREFIX}_${TARGET}_toolchain"
# TODO replace with linux/i686 when https://github.com/containers/buildah/issues/3252 will be fixed.
IMAGE_PLATFORM="linux/amd64"
IMAGE_LAYERS="false"
