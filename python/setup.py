#!/usr/bin/env python

from distutils.core import setup,Extension,os
import string

def cmd1(s):
    print s
    return os.popen(s).readlines()[0][:-1]

def cmd2(s):
    return string.split (cmd1(s))

setup(name = "TinySVM-python",
      version = cmd1("svm_model --version"),
      py_modules=["TinySVM"],
      ext_modules = [
        Extension("_TinySVM",
                  ["TinySVM_wrap.cxx",],
                  libraries=["tinysvm"])
        ])
