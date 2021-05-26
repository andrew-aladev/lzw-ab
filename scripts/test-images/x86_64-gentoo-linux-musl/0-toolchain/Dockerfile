ARG FROM_IMAGE
FROM $FROM_IMAGE

ARG TARGET
ENV TARGET=$TARGET

COPY prefix-root "/usr/${TARGET}"
RUN cp -R /mnt/toolchain-root/* /
RUN target-build.sh -v dev-libs/gmp dev-libs/libtommath && \
  build.sh -v dev-vcs/git dev-util/cmake && \
  target-cleanup.sh && cleanup.sh
