#!/usr/bin/env python3

import logging
import os
import platform
import shutil
import subprocess
from pathlib import Path


VCPKG_COMMIT = "c6db2c880a064b29e0b38eda29f19510235fb4c9"
VCPKG_LIBS = " ".join([
    "fmt",
    "spdlog",
])


# actions


def cd(path):
    logging.debug(f"change directory to '{os.path.realpath(path)}'")
    os.chdir(path)


def remove(path):
    logging.debug(f"removing '{path}'")
    shutil.rmtree(path, True)


def run(cmd):
    logging.debug(f"run: {cmd}")
    subprocess.check_call(cmd, shell=True)


def run_output(cmd):
    logging.debug(f"run, capturing output: {cmd}")
    return subprocess.check_output(cmd, shell=True)


# helpers


def is_win():
    return platform.system() == "Windows"


# steps


def install_vcpkg():
    logging.info("installing vcpkg")
    if os.path.exists(Path("vcpkg") / ".git"):
        logging.warning("vcpkg seems already installed, trying to update")
        if run_output(r'git -C "vcpkg" rev-parse HEAD').decode()[:-1] == VCPKG_COMMIT:
            logging.warning("vcpkg seems already up-to-date, skipping")
            return
        run(r'git -C "vcpkg" fetch -p')
    else:
        run(r'git clone "https://github.com/Microsoft/vcpkg.git"')
    run(rf'git -C "vcpkg" checkout "{VCPKG_COMMIT}"')
    bootstrap_file = "bootstrap-vcpkg.bat" if is_win() else "bootstrap-vcpkg.sh"
    run(rf'{Path("vcpkg") / bootstrap_file} -disableMetrics')


def install_vcpkg_packages():
    logging.info("installing vcpkg libs")
    triplet = "x64-windows" if is_win() else "x64-linux"
    vcpkg_exe = Path("vcpkg") / "vcpkg"
    run(rf'{vcpkg_exe} install --triplet {triplet} {VCPKG_LIBS}')
    run(rf'{vcpkg_exe} upgrade --no-dry-run')


def cleanup_vcpkg():
    logging.info("cleaning up vcpkg")
    vcpkg_path = Path("vcpkg")
    remove(vcpkg_path / "downloads")
    remove(vcpkg_path / "packages")


# main


if __name__ == "__main__":
    logging.basicConfig(datefmt="%H:%M:%S",
                        format="[%(asctime)s][%(levelname)s] %(message)s",
                        level=logging.DEBUG)
    cd(os.path.dirname(os.path.abspath(__file__)))

    logging.info(f"detected platform: {platform.system()}")

    install_vcpkg()
    install_vcpkg_packages()
    cleanup_vcpkg()
