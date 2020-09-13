ARG FROM_IMAGE=test_i686-unknown-linux-gnu_toolchain
FROM $FROM_IMAGE

ARG TARGET=i686-unknown-linux-gnu
ENV TARGET=$TARGET

COPY prefix-root "/usr/${TARGET}"
RUN target-build.sh -v dev-vcs/git dev-util/cmake dev-libs/gmp
