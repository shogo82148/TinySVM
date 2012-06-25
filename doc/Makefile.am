# Process this file with automake to produce Makefile.in

html_HTML = svm_learn.html svm_classify.html svm_model.html
EXTRA_DIST = $(html_HTML) index.html libtinysvm.html

update-html:
	@for html in $(html_HTML); do \
		man2html ../man/`basename $$html .html`.1 > $$html; \
	done



