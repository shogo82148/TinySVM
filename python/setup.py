#!/usr/bin/env python

# Copyright (C) 2020 Ichinose Shogo

from setuptools import setup, Extension
import subprocess

def cmd1(command):
    return subprocess.check_output(command, shell=True).decode().rstrip()

def cmd2(command):
    return cmd1(command).split()

setup(name = "TinySVM-python",
      version = cmd1("svm_model --version"),
      py_modules=["TinySVM"],
      ext_modules = [
        Extension("_TinySVM",
                  ["TinySVM_wrap.cxx",],
                  libraries=["tinysvm"])
        ])
