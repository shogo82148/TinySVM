#!/bin/sh

set -e
../src/svm_learn  train.svmdata model
../src/svm_classify test.svmdata model
../src/svm_model model
rm -f model
