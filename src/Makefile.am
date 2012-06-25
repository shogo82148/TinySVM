# Process this file with automake to produce Makefile.in
#
# $Id: Makefile.am,v 1.18 2001/12/06 12:58:54 taku-ku Exp $;
#
AUTOMAKE_OPTIONS = no-dependencies


lib_LTLIBRARIES = libtinysvm.la
libtinysvm_la_SOURCES = getopt.c getopt1.c getopt.h common.h \
			param.cc param.h \
			kernel.h \
			classifier.h classifier.cc \
		        q_matrix.h q_matrix.cc \
		        cache.h timer.h \
			base_example.cc base_example.h \
			example.cc example.h \
			model.cc model.h \
			base_solver.h \
			oneclass_solver.cc oneclass_solver.h \
			svm_solver.cc svm_solver.h \
			svr_solver.cc svr_solver.h \
			qp_solver.h qp_solver.cc \
			tinysvm.h

libtinysvm_la_LDFLAGS = -no-undefined -version-info $(LTVERSION)
include_HEADERS = tinysvm.h
pkginclude_HEADERS = model.h example.h base_example.h param.h \
                     misc.h kernel.h classifier.h

bin_PROGRAMS = svm_learn svm_classify svm_model

svm_learn_SOURCES = svm_learn.cc
svm_learn_LDADD = libtinysvm.la

svm_classify_SOURCES = svm_classify.cc
svm_classify_LDADD = libtinysvm.la

svm_model_SOURCES = svm_model.cc
svm_model_LDADD = libtinysvm.la

