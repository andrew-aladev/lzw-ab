ARG FROM_IMAGE=test_x86_64-gentoo-linux-musl_toolchain
FROM $FROM_IMAGE

ARG TARGET=x86_64-gentoo-linux-musl
ENV TARGET=$TARGET

COPY prefix-root "/usr/${TARGET}"
RUN target-build.sh -v dev-vcs/git dev-util/cmake dev-libs/gmp
