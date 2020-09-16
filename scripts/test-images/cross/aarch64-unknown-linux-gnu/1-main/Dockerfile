ARG FROM_IMAGE=test_aarch64-unknown-linux-gnu
FROM $FROM_IMAGE

COPY root /
COPY attached-main-root /
RUN build.sh -v dev-vcs/git dev-util/cmake dev-libs/gmp
