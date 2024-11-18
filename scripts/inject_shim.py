import os
import subprocess
import sys

def inject_shim(initramfs_path, shim_path, output_path):
    if not os.path.isfile(initramfs_path) or not os.path.isfile(shim_path):
        raise FileNotFoundError("Invalid file path for initramfs or shim binary")

    temp_dir = "./temp_initramfs"
    os.makedirs(temp_dir, exist_ok=True)

    with open(initramfs_path, "rb") as initramfs_file:
        subprocess.run(["cpio", "-id"], cwd=temp_dir, input=initramfs_file.read())

    subprocess.run(["cp", shim_path, f"{temp_dir}/shim.ko"], check=True)

    with open(output_path, "wb") as output_file:
        
