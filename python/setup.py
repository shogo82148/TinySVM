#!/usr/bin/env python

# Copyright (C) 2020 Ichinose Shogo

from setuptools import setup, Extension
import re
import subprocess

def cmd1(command):
    return subprocess.check_output(command, shell=True).decode().rstrip()

def cmd2(command):
    return cmd1(command).split()

def version():
    # "svm_model --version" prints "svm_model of 0.09" followed by banner lines
    return re.search(r"\d+(\.\d+)+", cmd1("svm_model --version")).group(0)

setup(name = "TinySVM-python",
      version = version(),
      py_modules=["TinySVM"],
      ext_modules = [
        Extension("_TinySVM",
                  ["TinySVM_wrap.cxx",],
                  libraries=["tinysvm"])
        ])
