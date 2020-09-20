ARG FROM_IMAGE=test_aarch64-unknown-linux-gnu_toolchain
FROM $FROM_IMAGE

ARG TARGET=aarch64-unknown-linux-gnu
ENV TARGET=$TARGET

COPY prefix-root "/usr/${TARGET}"
COPY attached-toolchain-root /
RUN build.sh -v dev-vcs/git dev-util/cmake && \
  target-build.sh -v dev-libs/gmp

RUN target-cleanup.sh
