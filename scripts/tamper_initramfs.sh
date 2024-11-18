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

COMPRESSION=$(file -b "$INITRAMFS_PATH" | grep -oE '(gzip|xz|lz4)')
case "$COMPRESSION" in
    gzip)
        gunzip -c "$INITRAMFS_PATH" | (cd "$TEMP_DIR" && cpio -id)
        ;;
    xz)
        xz -dc "$INITRAMFS_PATH" | (cd "$TEMP_DIR" && cpio -id)
        ;;
    lz4)
        lz4 -dc "$INITRAMFS_PATH" | (cd "$TEMP_DIR" && cpio -id)
        ;;
    *)
        echo "Unsupported initramfs compression format"
        exit 1
        ;;
esac

cp "$SHIM_BINARY" "$TEMP_DIR/shim.ko"
cp "shim_hooks.txt" "$TEMP_DIR/shim_hooks.txt"
(cd "$TEMP_DIR" && find . | cpio -o -H newc | gzip > "../$OUTPUT_INITRAMFS")
rm -rf "$TEMP_DIR"
