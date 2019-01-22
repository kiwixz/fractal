#!/usr/bin/env python3

import collections
import json
import logging
import os
import re
import shutil
import subprocess
import tempfile
import zipfile
from pathlib import Path


APP_NAME = "fractal"
EXE_FILES = [f"{APP_NAME}.exe"]
EXTRA_FILES = ["LICENSE"]


def find_vs_path():
    logging.info("searching VS path")
    vswhere_exe = str(Path(os.environ["ProgramFiles(x86)"]) / "Microsoft Visual Studio" / "Installer" / "vswhere.exe")
    vswhere_output = subprocess.check_output([vswhere_exe, "-latest", "-format", "json", "-property", "installationPath"])
    vs_path = json.loads(vswhere_output)[0]["installationPath"]
    logging.info(f"found VS path: '{vs_path}'")
    return vs_path


def last_version(path):
    versions = os.listdir(path)
    versions.sort(key=lambda version: [int(n) if n.isdigit() else 0
                                       for n in version.split(".")])
    return path / versions[-1]


def build_app(vs_path):
    logging.info("building solution")
    msbuild_path = Path(vs_path) / "MSBuild"
    msbuild_exe = str(last_version(msbuild_path) / "Bin" / "MSBuild.exe")
    subprocess.check_call([msbuild_exe, "/m", "/v:m", "/p:Configuration=Release", "/p:Platform=x64"])


def find_required_dlls():
    logging.info("searching required DLLs")
    tools_path = Path(vs_path) / "VC" / "Tools" / "MSVC"
    dumpbin_exe = str(last_version(tools_path) / "bin" / "Hostx64" / "x64" / "dumpbin.exe")
    sources = collections.deque([Path("build") / "x64-Release" / file for file in EXE_FILES])
    dlls_list = set()
    while len(sources):
        source = str(sources.pop())
        raw = subprocess.check_output([dumpbin_exe, "/DEPENDENTS", source])
        matches = re.findall(r'(^|\s)([\w.-]+\.dll)($|\s)', raw.decode())
        new_dlls = [match[1] for match in matches]
        sources.extend([dll for dll in set([Path("build") / "x64-Release" / dll
                                            for dll in new_dlls]).difference(dlls_list)
                        if dll.exists()])
        dlls_list = dlls_list.union(new_dlls)
    return dlls_list


def copy_app(package_dir, required_dlls):
    logging.info(f"copying app")
    bin_source = Path("build") / "x64-Release"
    for exe in [bin_source / file for file in EXE_FILES]:
        shutil.copy2(exe, package_dir)
    for dll in bin_source.glob("**/*.dll"):
        if dll.name.lower() in required_dlls:
            shutil.copy2(dll, package_dir)
    for file in EXTRA_FILES:
        path = Path(file)
        if path.is_dir():
            shutil.copytree(file, package_dir / path.name)
        else:
            shutil.copy2(file, package_dir)


def copy_redist(vs_path, package_dir, required_dlls):
    logging.info("copying redist")
    redist_path = Path(vs_path) / "VC" / "Redist" / "MSVC"
    redist_path = last_version(redist_path) / "x64"
    for dll in redist_path.glob("**/*.dll"):
        if dll.name.lower() in required_dlls:
            shutil.copy2(dll, package_dir)


def zip_app(package_dir):
    logging.info(f"zipping")
    with zipfile.ZipFile(f"build/{APP_NAME}.zip", "w", zipfile.ZIP_LZMA) as zipf:
        for file in package_dir.glob("**/*"):
            if file.is_file():
                zipf.write(file, f"{APP_NAME}/{file.relative_to(package_dir)}")


if __name__ == "__main__":
    logging.basicConfig(datefmt="%H:%M:%S",
                        format="[%(asctime)s][%(levelname)s] %(message)s",
                        level=logging.DEBUG)
    os.chdir(os.path.dirname(os.path.abspath(__file__)))

    vs_path = find_vs_path()
    build_app(vs_path)
    required_dlls = [dll.lower() for dll in find_required_dlls()]

    package_dir = Path(tempfile.mkdtemp())
    try:
        copy_app(package_dir, required_dlls)
        copy_redist(vs_path, package_dir, required_dlls)
        zip_app(package_dir)
    finally:
        shutil.rmtree(package_dir)
