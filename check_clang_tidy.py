#!/usr/bin/env python3

import difflib
import logging
import os
import subprocess
import sys
from pathlib import Path


if __name__ == "__main__":
    logging.basicConfig(datefmt="%H:%M:%S",
                        format="[%(asctime)s][%(levelname)s] %(message)s",
                        level=logging.DEBUG,
                        stream=sys.stderr)
    os.chdir(os.path.dirname(os.path.abspath(__file__)))

    build_dir = sys.argv[1] if len(sys.argv) > 1 else "build"
    if not (Path(build_dir) / "compile_commands.json").exists():
        logging.critical("compile_commands.json not found, did you pass the correct build directory ?")
        exit(2)

    exit_code = 0
    git_files = subprocess.check_output(["git", "ls-files"], text=True).splitlines()
    for path in git_files:
        if path.endswith(".cpp"):
            logging.info(f"checking clang-format: {path}")
            subprocess.check_call(["clang-tidy", "-p", build_dir, path])
    exit(exit_code)
