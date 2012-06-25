# Process this file with automake to produce Makefile.in
#
# $Id: Makefile.am,v 1.9 2000/12/09 05:05:47 taku-ku Exp $;
#
AUTOMAKE_OPTIONS = no-dependencies

lib_LTLIBRARIES = libtinysvm.la
libtinysvm_la_SOURCES = getopt.c getopt1.c getopt.h common.h \
			param.cc param.h kernel.h\
			classifier.h classifier.cc \
			gram_matrix.h gram_matrix.cc \
		        cache.h timer.h \
			base_example.cc base_example.h \
			example.cc example.h model.cc model.h \
			base_solver.h svm_solver.cc svm_solver.h \
			tinysvm.h

libtinysvm_la_LDFLAGS = -version-info $(LTVERSION)
libtinysvm_la_LDDADD = -lm
include_HEADERS = tinysvm.h
pkginclude_HEADERS = model.h example.h base_example.h param.h misc.h

bin_PROGRAMS = svm_learn svm_classify svm_model

svm_learn_SOURCES = svm_learn.cc
svm_learn_LDADD = libtinysvm.la -lm

svm_classify_SOURCES = svm_classify.cc
svm_classify_LDADD = libtinysvm.la -lm

svm_model_SOURCES = svm_model.cc
svm_model_LDADD = libtinysvm.la -lm

