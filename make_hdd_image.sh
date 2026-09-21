#!/usr/bin/env bash

set -euo pipefail

# ============================================================================
# DUX HDD image builder
#
# Creates a BIOS-bootable HDD image:
#
#   MBR
#    └── GRUB2 (i386-pc)
#         └── ext2
#              └── /boot/dux
#
# Usage:
#   ./make_hdd_image.sh
# ============================================================================

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

KERNEL="${ROOT_DIR}/build-i686/dux"

IMAGE_DIR="${ROOT_DIR}/image"
IMAGE="${IMAGE_DIR}/dux-hdd.img"
MOUNT_DIR="${IMAGE_DIR}/mnt"

IMAGE_SIZE="64M"
VOLUME_LABEL="DUX"

LOOP=""

# ----------------------------------------------------------------------------
# Helpers
# ----------------------------------------------------------------------------

die()
{
    echo "ERROR: $*" >&2
    exit 1
}

info()
{
    echo
    echo "==> $*"
}

cleanup()
{
    set +e

    if mountpoint -q "${MOUNT_DIR}" 2>/dev/null; then
        info "Unmounting ${MOUNT_DIR}"
        sudo umount "${MOUNT_DIR}"
    fi

    if [[ -n "${LOOP}" ]]; then
        if sudo losetup "${LOOP}" >/dev/null 2>&1; then
            info "Detaching ${LOOP}"
            sudo losetup -d "${LOOP}"
        fi
    fi
}

trap cleanup EXIT INT TERM


# ----------------------------------------------------------------------------
# Check dependencies
# ----------------------------------------------------------------------------

info "Checking dependencies"

REQUIRED_TOOLS=(
    grub-install
    grub-file
    parted
    losetup
    mkfs.ext2
    mount
    mountpoint
    truncate
)

for tool in "${REQUIRED_TOOLS[@]}"; do
    command -v "${tool}" >/dev/null 2>&1 ||
        die "Required tool not found: ${tool}"
done


# ----------------------------------------------------------------------------
# Check kernel
# ----------------------------------------------------------------------------

info "Checking DUX kernel"

[[ -f "${KERNEL}" ]] ||
    die "Kernel not found: ${KERNEL}"

if ! grub-file --is-x86-multiboot2 "${KERNEL}"; then
    die "Kernel is not a valid Multiboot2 image: ${KERNEL}"
fi

echo "Kernel:     ${KERNEL}"
echo "Multiboot2: OK"


# ----------------------------------------------------------------------------
# Prepare directories
# ----------------------------------------------------------------------------

info "Preparing image directory"

mkdir -p "${IMAGE_DIR}"
mkdir -p "${MOUNT_DIR}"

# Do not accidentally overwrite a mounted image from an earlier run.
if mountpoint -q "${MOUNT_DIR}"; then
    die "${MOUNT_DIR} is already mounted"
fi


# ----------------------------------------------------------------------------
# Create HDD image
# ----------------------------------------------------------------------------

info "Creating ${IMAGE_SIZE} HDD image"

rm -f "${IMAGE}"

truncate -s "${IMAGE_SIZE}" "${IMAGE}"

parted -s "${IMAGE}" \
    mklabel msdos \
    mkpart primary ext2 1MiB 100% \
    set 1 boot on

echo
parted "${IMAGE}" unit s print


# ----------------------------------------------------------------------------
# Create loop device
# ----------------------------------------------------------------------------

info "Attaching loop device"

LOOP="$(sudo losetup --find --show --partscan "${IMAGE}")"

echo "Loop device: ${LOOP}"

PARTITION="${LOOP}p1"

# Give the kernel a moment to expose the partition device.
for _ in {1..20}; do
    [[ -b "${PARTITION}" ]] && break
    sleep 0.1
done

[[ -b "${PARTITION}" ]] ||
    die "Partition device was not created: ${PARTITION}"

lsblk "${LOOP}"


# ----------------------------------------------------------------------------
# Create filesystem
# ----------------------------------------------------------------------------

info "Creating ext2 filesystem"

sudo mkfs.ext2 -F \
    -L "${VOLUME_LABEL}" \
    "${PARTITION}"


# ----------------------------------------------------------------------------
# Mount filesystem
# ----------------------------------------------------------------------------

info "Mounting filesystem"

sudo mount "${PARTITION}" "${MOUNT_DIR}"

sudo mkdir -p \
    "${MOUNT_DIR}/boot/grub"


# ----------------------------------------------------------------------------
# Install kernel
# ----------------------------------------------------------------------------

info "Installing DUX kernel"

sudo cp "${KERNEL}" "${MOUNT_DIR}/boot/dux"


# ----------------------------------------------------------------------------
# GRUB configuration
# ----------------------------------------------------------------------------

info "Creating GRUB configuration"

sudo tee "${MOUNT_DIR}/boot/grub/grub.cfg" >/dev/null <<'EOF'
set timeout=3
set default=0

menuentry "DUX" {
    multiboot2 /boot/dux
    boot
}
EOF


# ----------------------------------------------------------------------------
# Install GRUB2
# ----------------------------------------------------------------------------

info "Installing GRUB2"

sudo grub-install \
    --target=i386-pc \
    --boot-directory="${MOUNT_DIR}/boot" \
    --modules="part_msdos ext2 multiboot2" \
    --no-floppy \
    "${LOOP}"


# ----------------------------------------------------------------------------
# Flush filesystem
# ----------------------------------------------------------------------------

info "Flushing filesystem"

sync

sudo umount "${MOUNT_DIR}"

sudo losetup -d "${LOOP}"
LOOP=""


# ----------------------------------------------------------------------------
# Result
# ----------------------------------------------------------------------------

info "DUX HDD image created successfully"

echo
echo "Image:"
echo "  ${IMAGE}"
echo
echo "Size:"
du -h "${IMAGE}"

echo
echo "Ready for Bochs."
