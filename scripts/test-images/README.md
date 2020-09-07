# Gentoo images for software testing

You can find them on https://hub.docker.com/u/puchuu.

## Dependencies

- [buildah](https://github.com/containers/buildah)
- [qemu](https://github.com/qemu/qemu) `QEMU_USER_TARGETS="aarch64 aarch64_be arm armeb mips mipsel"`

## Build

Packages for cross architectures are building using qemu static user, compilation is heavy.
Recommended CPU is any modern one with >= `4 cores`.
Max required RAM ~ `2 GB` per core.

Please start `qemu-binfmt` service.

Than add your local user to `/etc/subuid` and `/etc/subgid`:

```
my_user:100000:65536
```

Than open [`env.sh`](env.sh) and update variables.

```sh
./build.sh
./push.sh
./pull.sh
./run.sh
```

Build is rootless, just use your regular `my_user`.
