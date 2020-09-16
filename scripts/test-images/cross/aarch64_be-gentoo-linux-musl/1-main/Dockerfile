ARG FROM_IMAGE=test_aarch64_be-gentoo-linux-musl
FROM $FROM_IMAGE

COPY root /
COPY attached-main-root /
RUN build.sh -v dev-vcs/git dev-util/cmake dev-libs/gmp
