#!/bin/bash
set -e

INITRAMFS_PATH="$1"
SHIM_BINARY="$2"
OUTPUT_INITRAMFS="$3"

if [ -z "$INITRAMFS_PATH" ] || [ -z "$SHIM_BINARY" ] || [ -z "$OUTPUT_INITRAMFS" ]; then
    echo "Usage: $0 <initramfs_path> <shim_binary> <output_initramfs>"
    exit 1
fi

TEMP_DIR="./temp_initramfs"
mkdir -p "$TEMP_DIR"

cd "$TEMP_DIR"
gzip -dc "$INITRAMFS_PATH" | cpio -id
cp "$SHIM_BINARY" "./shim.ko"
find . | cpio -o -H newc | gzip > "../$OUTPUT_INITRAMFS"
cd -
rm -rf "$TEMP_DIR"
