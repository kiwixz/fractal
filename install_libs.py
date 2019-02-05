#!/usr/bin/env python3

import logging
import os
import platform
import shutil
import subprocess
from distutils import dir_util
from pathlib import Path


VCPKG_COMMIT = "61d163a8fcdfec87ae90efbb9b107c3b0372492c"
VCPKG_LIBS = [
    "cxxopts",
    "fmt",
    "glad",
    "glfw3",
    "libvpx",
    "spdlog",
    "tinyfiledialogs",
]


def is_win():
    return platform.system() == "Windows"


def install_vcpkg():
    logging.info("installing vcpkg")
    if os.path.exists(Path("vcpkg") / ".git"):
        logging.warning("vcpkg seems already installed, trying to update")
        if subprocess.check_output(["git", "-C", "vcpkg", "rev-parse", "HEAD"]).decode()[:-1] == VCPKG_COMMIT:
            logging.warning("vcpkg seems already up-to-date, skipping")
            return
        subprocess.check_call(["git", "-C", "vcpkg", "fetch", "-p"])
    else:
        subprocess.check_call(["git", "clone", "https://github.com/Microsoft/vcpkg.git"])
    subprocess.check_call(["git", "-C", "vcpkg", "checkout", VCPKG_COMMIT])
    bootstrap_file = str(Path("vcpkg") / ("bootstrap-vcpkg.bat" if is_win() else "bootstrap-vcpkg.sh"))
    subprocess.check_call([bootstrap_file, "-disableMetrics"])


def install_vcpkg_packages():
    logging.info("installing vcpkg libs")
    dir_util.copy_tree("vcpkg_config", "vcpkg")
    triplet = "x64-windows" if is_win() else "x64-linux"
    vcpkg_exe = str(Path("vcpkg") / "vcpkg")
    subprocess.check_call([vcpkg_exe, "install", "--triplet", triplet] + VCPKG_LIBS)
    subprocess.check_call([vcpkg_exe, "upgrade", "--no-dry-run"])


def cleanup_vcpkg():
    logging.info("cleaning up vcpkg")
    vcpkg_path = Path("vcpkg")
    shutil.rmtree(vcpkg_path / "buildtrees", True)
    shutil.rmtree(vcpkg_path / "packages", True)
    for file in (vcpkg_path / "downloads").glob("*"):
        if file.is_file():
            file.unlink()


if __name__ == "__main__":
    logging.basicConfig(datefmt="%H:%M:%S",
                        format="[%(asctime)s][%(levelname)s] %(message)s",
                        level=logging.DEBUG)
    os.chdir(os.path.dirname(os.path.abspath(__file__)))

    logging.info(f"detected platform: {platform.system()}")

    install_vcpkg()
    install_vcpkg_packages()
    cleanup_vcpkg()
