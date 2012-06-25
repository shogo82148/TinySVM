# Process this file with automake to produce Makefile.in
#
# $Id: Makefile.am,v 1.20 2001/01/14 02:55:27 taku-ku Exp $;
#
@SET_MAKE@
AUTOMAKE_OPTIONS = 1.4 no-dependencies

always_built_SUBDIRS = src tests man doc

EXTRA_DIRS = doc perl ruby

EXTRA_DIST = tinysvm.spec \
             src/Makefile.bcc32 config.h.bcc32 \
             src/Makefile.mingw32msvc config.h.mingw32msvc

SUBDIRS = $(always_built_SUBDIRS)

pkgdatadir = @datadir@/@PACKAGE@

dist-hook:
	for subdir in $(EXTRA_DIRS); do \
		cp -rp $$subdir $(distdir)/$$subdir; \
		rm -f $(distdir)/$$subdir/*~; \
		rm -f $(distdir)/$$subdir/*.{bak,orig,old}; \
		rm -f $(distdir)/$$subdir/Makefile.PL; \
		rm -rf $(distdir)/$$subdir/CVS; \
		rm -rf $(distdir)/$$subdir/*/CVS; \
	done

rpm: dist
	rpm -ta @PACKAGE@-@VERSION@.tar.gz
	
win:
	 cd src; make -f Makefile.mingw32msvc clean; make -f Makefile.mingw32msvc;
	 
win-dist: dist
	rm -f @PACKAGE@-@VERSION@.zip
	mkdir bin; cp src/*.exe bin/
	zip @PACKAGE@-@VERSION@.zip README AUTHORS NEWS COPYING ChangeLog \
	bin/*.exe doc/*.html doc/*.css tests/*.svmdata
	rm -fr bin/
