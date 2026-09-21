#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"

TOOLCHAIN_ROOT="${HOME}/opt/cross"
TOOLCHAIN_BIN="${TOOLCHAIN_ROOT}/bin"
TARGET_ROOT="${TOOLCHAIN_ROOT}/i686-elf"

CC="${TOOLCHAIN_BIN}/i686-elf-gcc"
CXX="${TOOLCHAIN_BIN}/i686-elf-g++"
AS="${TOOLCHAIN_BIN}/i686-elf-gcc"

# Corresponds to:
#
#   set(CMAKE_SYSTEM_NAME Generic)
#   set(CMAKE_SYSTEM_PROCESSOR i686)
#
export ARCH=x86
export OSDEV_FREESTANDING=1

# Corresponds to the compiler configuration from i686-elf.cmake.
export CC
export CXX
export AS

# Keep the cross toolchain first in PATH.
export PATH="${TOOLCHAIN_BIN}:${PATH}"

# Equivalent target root information for tools/scripts that may need it.
export CROSS_ROOT="${TARGET_ROOT}"

# Number of parallel jobs.
JOBS="${JOBS:-$(nproc)}"

cd "${ROOT_DIR}"

exec make \
    ARCH="${ARCH}" \
    OSDEV_FREESTANDING="${OSDEV_FREESTANDING}" \
    CC="${CC}" \
    CXX="${CXX}" \
    AS="${AS}" \
    -j"${JOBS}" \
    "$@"