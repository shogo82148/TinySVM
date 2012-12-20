# Process this file with automake to produce Makefile.in
#
# $Id: Makefile.am,v 1.26 2001/12/07 10:54:21 taku-ku Exp $;
#
@SET_MAKE@
AUTOMAKE_OPTIONS = 1.4 no-dependencies

always_built_SUBDIRS = src tests man doc
EXTRA_DIST = tinysvm.spec THANKS \
swig/Makefile \
swig/TinySVM.i \
ruby/README \
ruby/test.rb \
ruby/TinySVM_wrap.cpp \
ruby/extconf.rb.in \
perl/README \
perl/example/hoge \
perl/example/model \
perl/example/svm_classify.pl \
perl/example/svm_learn.pl \
perl/TinySVM_wrap.cxx \
perl/TinySVM.pm \
perl/Makefile.PL.in \
java/org/chasen/tinysvm/SWIGTYPE_p_TinySVM__feature_node.java \
java/org/chasen/tinysvm/TinySVMConstants.java \
java/org/chasen/tinysvm/TinySVM.java \
java/org/chasen/tinysvm/SWIGTYPE_p_FILE.java \
java/org/chasen/tinysvm/Model.java \
java/org/chasen/tinysvm/BaseExample.java \
java/org/chasen/tinysvm/SWIGTYPE_p_TinySVM__Param.java \
java/org/chasen/tinysvm/TinySVMJNI.java \
java/org/chasen/tinysvm/SWIGTYPE_p_double.java \
java/org/chasen/tinysvm/Example.java \
java/README \
java/TinySVM_wrap.cxx \
java/Makefile.in \
java/test.java \
python/README \
python/test.py \
python/TinySVM_wrap.cxx \
python/TinySVM.py \
python/setup.py


pkgconfigdir   = $(libdir)/pkgconfig
pkgconfig_DATA = tinysvm.pc

SUBDIRS = $(always_built_SUBDIRS)
ACLOCAL_AMFLAGS = -I m4
pkgdatadir = @datadir@/@PACKAGE@

clean-swig:
	(cd perl; rm -f Makefile.PL Makefile.old model* *.so *.o Makefile)
	(cd ruby; rm -f extconf.rb model* Makefile *.o *.so)
	(cd python; rm -fr Makefile Makefile.pre Setup *.o model* config.c sedscript *.pyc *.so)
	(cd java; rm -fr *.class *.jar model* Makefile *.so *.o)

update-man:
	make -C man update-man
	make -C doc update-html

rpm: dist
	rpm -ta @PACKAGE@-@VERSION@.tar.gz

win:
	make distclean
	env AR=i386-mingw32msvc-ar RANLIB=i386-mingw32msvc-ranlib CXX=i386-mingw32msvc-g++ CC=i386-mingw32msvc-gcc \
	./configure --host=i386-pc-mingw32  --target=i386-pc-mingw32 --disable-shared
	make

win-dist:
	rm -f @PACKAGE@-@VERSION@.zip
	mkdir bin;
	cp -f src/svm_learn.exe    bin/svm_learn.exe
	cp -f src/svm_classify.exe bin/svm_classify.exe
	cp -f src/svm_model.exe    bin/svm_model.exe

	zip @PACKAGE@-@VERSION@.zip README AUTHORS NEWS COPYING THANKS ChangeLog \
	bin/*.exe doc/*.html doc/*.css tests/*.svmdata
	rm -fr bin/

dist-all-package:
	(test -f Makefile) && make distclean
	./configure
	make dist
	make rpm
	make clean
	make win
	make win-dist
