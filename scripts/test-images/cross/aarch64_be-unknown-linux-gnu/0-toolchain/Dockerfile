ARG FROM_IMAGE=test_aarch64_be-unknown-linux-gnu_toolchain
FROM $FROM_IMAGE

ARG TARGET=aarch64_be-unknown-linux-gnu
ENV TARGET=$TARGET

COPY prefix-root "/usr/${TARGET}"
RUN target-build.sh -v dev-vcs/git dev-util/cmake dev-libs/gmp
