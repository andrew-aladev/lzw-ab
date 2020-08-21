# Gentoo images for software testing

You can find them on https://hub.docker.com/u/puchuu.

## Dependencies

- [docker cli](https://github.com/docker/cli)
- [buildah](https://github.com/containers/buildah)
- [qemu](https://github.com/qemu/qemu) `QEMU_USER_TARGETS="aarch64 aarch64_be arm armeb mips mipsel"`

## Build

Packages for cross architectures are building using qemu static user, compilation is heavy.
Recommended CPU is any modern one with >= `4 cores`.
Max required RAM ~ `2 GB` per core.

Please start `docker` and `qemu-binfmt` services.

Than allow other users in `/etc/fuse.conf`:

```
user_allow_other
```

Than add your local user to `/etc/subuid` and `/etc/subgid`:

```
my_user:100000:65536
```

Please ensure that your local user is in `docker` group.

Than open [`env.sh`](env.sh) and update variables.

```sh
./build.sh
./push.sh
./pull.sh
./run.sh
```

Build is rootless, just use your regular `my_user`.
