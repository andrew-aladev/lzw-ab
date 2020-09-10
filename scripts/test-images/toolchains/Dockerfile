ARG FROM_IMAGE=stage3-amd64-nomultilib
FROM $FROM_IMAGE

ARG PORTAGE_SNAPSHOT=latest
ARG TARGETS=x86_64-unknown-linux-gnu
ENV PORTAGE_SNAPSHOT=$PORTAGE_SNAPSHOT
ENV TARGETS=$TARGETS

COPY root/ /
RUN env-update && emerge-webrsync --revert="${PORTAGE_SNAPSHOT}" && \
  eselect profile set default/linux/amd64/17.1/no-multilib && \
  env-update && source /etc/profile

RUN for TARGET in TARGETS; do \
  done
