#!/usr/bin/env python3

import glob
import logging
import os
import subprocess


# actions


def cd(path):
    logging.debug(f"changing directory to '{os.path.realpath(path)}'")
    os.chdir(path)


def run(cmd):
    logging.debug(f"running: {cmd}")
    subprocess.check_call(cmd, shell=True)


def run_output(cmd):
    logging.debug(f"running, capturing output: {cmd}")
    return subprocess.check_output(cmd, shell=True)


# main


if __name__ == "__main__":
    logging.basicConfig(datefmt="%H:%M:%S",
                        format="[%(asctime)s][%(levelname)s] %(message)s",
                        level=logging.DEBUG)
    cd(os.path.dirname(os.path.abspath(__file__)))

    git_files = run_output(r'git ls-files').decode().splitlines()
    for f in git_files:
        if f.endswith(".cpp") or f.endswith(".h") or f.endswith(".inl"):
            logging.info(f"formatting: {f}")
            run(rf'clang-format -i {f}')
