#!/usr/bin/env python3

import difflib
import logging
import os
import subprocess
import sys


if __name__ == "__main__":
    logging.basicConfig(datefmt="%H:%M:%S",
                        format="[%(asctime)s][%(levelname)s] %(message)s",
                        level=logging.DEBUG,
                        stream=sys.stderr)
    os.chdir(os.path.dirname(os.path.abspath(__file__)))

    exit_code = 0
    git_files = subprocess.check_output(["git", "ls-files"], text=True).splitlines()
    for path in git_files:
        if path.endswith(".cpp") or path.endswith(".h"):
            logging.info(f"checking clang-format: {path}")
            with open(path) as f:
                original = f.read()
            formatted = subprocess.check_output(["clang-format", "-assume-filename", path],
                                                input=original, text=True)
            diff = difflib.unified_diff(original.splitlines(keepends=True),
                                        formatted.splitlines(keepends=True),
                                        path,
                                        "formatted")
            diff_result = list(diff)
            if diff_result:
                exit_code = 1
                logging.warning("differences found:")
                sys.stdout.writelines(diff_result)
                print()
    exit(exit_code)
