# Process this file with automake to produce Makefile.in

man_MANS = svm_learn.1 svm_classify.1 svm_model.1
EXTRA_DIST = $(man_MANS)

update-man:
	man=svm_learn.1; LC_ALL=C help2man --no-info $(top_builddir)/src/`basename $$man .1` -m "TinySVM" -n "learns SVM models" | \
		perl -pe 's/(LT|lt)-//g' > $$man;
	man=svm_classify.1; LC_ALL=C help2man --no-info $(top_builddir)/src/`basename $$man .1` -m "TinySVM" -n "classifies data according to a SVM model" | \
		perl -pe 's/(LT|lt)-//g' > $$man;
	man=svm_model.1; LC_ALL=C help2man --no-info $(top_builddir)/src/`basename $$man .1` -m "TinySVM" -n "shows SVM models information" | \
		perl -pe 's/(LT|lt)-//g' > $$man;

