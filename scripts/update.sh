#!/usr/bin/env bash
set -e

DIR=$(dirname "${BASH_SOURCE[0]}")
cd "$DIR"

git fetch --all || :
git fetch --tags || :
git remote | xargs -I {} git rebase "{}/$(git branch --show-current)" || :

git submodule update --init --recursive --remote
