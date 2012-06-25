# Process this file with automake to produce Makefile.in
#
# $Id: Makefile.am,v 1.15 2000/12/09 06:19:15 taku-ku Exp $;
#
@SET_MAKE@
AUTOMAKE_OPTIONS = 1.4 no-dependencies

always_built_SUBDIRS = src tests man

EXTRA_DIRS = doc perl

EXTRA_DIST = tinysvm.spec Makefile.bcc32 config.h.bcc32

SUBDIRS = $(always_built_SUBDIRS)

pkgdatadir = @datadir@/@PACKAGE@

dist-hook:
	cd perl; make clean; rm -f model; cd ..
	cd man;  make update-man; cd ..
	cd html; make update-html; cd ..
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

win-dist: dist
	rm -f @PACKAGE@-@VERSION@.zip 
	zip @PACKAGE@-@VERSION@.zip README AUTHORS NEWS COPYING ChangeLog \
	src/*.exe doc/*.html doc/*.css tests/*.svmdata
