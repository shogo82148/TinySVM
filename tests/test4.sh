#!/bin/sh

set -e
../src/svm_learn -l 1 -t 1 -d 2 -c 1 train.svrdata model
../src/svm_model model
rm -f model
