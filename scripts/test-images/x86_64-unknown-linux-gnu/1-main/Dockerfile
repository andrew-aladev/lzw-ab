ARG FROM_IMAGE
FROM $FROM_IMAGE

COPY root /
RUN cp -R /mnt/main-root/* /
RUN build.sh -v dev-vcs/git dev-util/cmake dev-libs/gmp dev-libs/libtommath && \
  cleanup.sh
