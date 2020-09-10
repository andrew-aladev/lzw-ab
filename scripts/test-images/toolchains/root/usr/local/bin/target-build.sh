#!/bin/bash
set -e

DIR=$(dirname "${BASH_SOURCE[0]}")
source "/etc/profile.d/makeopts.sh"

"${TARGET}-emerge" "$@"
