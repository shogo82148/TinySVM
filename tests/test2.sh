#!/bin/sh

../src/svm_learn -t 1 -d 2 -c 1 train.svmdata model
../src/svm_classify test.svmdata model
../src/svm_model model
rm -f model
