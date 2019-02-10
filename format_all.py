#!/usr/bin/env python3

import logging
import os
import subprocess


if __name__ == "__main__":
    logging.basicConfig(datefmt="%H:%M:%S",
                        format="[%(asctime)s][%(levelname)s] %(message)s",
                        level=logging.DEBUG)
    os.chdir(os.path.dirname(os.path.abspath(__file__)))

    git_files = subprocess.check_output(["git", "ls-files"]).decode().splitlines()
    for f in git_files:
        if f.endswith(".cpp") or f.endswith(".h") or f.endswith(".inl"):
            logging.info(f"formatting: {f}")
            subprocess.check_call(["clang-format", "-i", f])
