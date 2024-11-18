import os
import subprocess
import json
import sys

def load_config(config_file):
    with open(config_file, "r") as file:
        return json.load(file)

def inject_shim(config):
    initramfs = config["initramfs"]
    shim_path = config["shim"]
    output_path = config["output"]
    hooks = config.get("hooks", [])

    with open("shim_hooks.txt", "w") as hook_file:
        for hook in hooks:
            hook_file.write(hook + "\n")

    subprocess.run(["cpio", "-iv"], input=f"{shim_path}".encode(), cwd=initramfs, check=True)
    subprocess.run(["cp", "shim_hooks.txt", f"{initramfs}/shim_hooks.txt"], check=True)
    subprocess.run(["find", ".", "|", "cpio", "-o", "-H", "newc"], cwd=initramfs, check=True, stdout=open(output_path, "wb"))

def main():
    if len(sys.argv) != 2:
        print("Usage: inject_shim.py <config_file>")
        sys.exit(1)

    config = load_config(sys.argv[1])
    inject_shim(config)

if __name__ == "__main__":
    main()
