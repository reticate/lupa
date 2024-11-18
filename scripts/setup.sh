#!/bin/bash
set -e

KERNEL_VERSION=$(uname -r)
INITRAMFS_PATH="$1"
BOOT_DIR="/boot"
OUTPUT_INITRAMFS="$BOOT_DIR/initramfs-$KERNEL_VERSION-custom.img"

if [ -z "$INITRAMFS_PATH" ]; then
    echo "Usage: $0 <modified_initramfs_path>"
    exit 1
fi

if [ ! -f "$INITRAMFS_PATH" ]; then
    echo "Initramfs file not found at $INITRAMFS_PATH"
    exit 1
fi

cp "$INITRAMFS_PATH" "$OUTPUT_INITRAMFS"
update-grub
