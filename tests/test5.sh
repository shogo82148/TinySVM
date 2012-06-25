#!/bin/sh

head -900 train.svmdata > train.900
../src/svm_learn -I train.900 model.900
../src/svm_learn -M model.900 train.svmdata model

head -990 train.svmdata > train.990
../src/svm_learn -I train.990 model.990
../src/svm_learn -M model.990 train.svmdata model

head -998 train.svmdata > train.998
../src/svm_learn -I train.998 model.998
../src/svm_learn -M model.998 train.svmdata model

head -500 train.svmdata > train.500
../src/svm_learn -I train.500 model.500
../src/svm_learn -M model.500 train.svmdata model

rm -f model train.9* model.9* model.500.* train.500
