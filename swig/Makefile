SWIG = swig
PREFIX = TinySVM

all: perl ruby python java

clean:
	rm -f *.pm *.cxx *.py

.PHONY: all clean perl ruby python java

perl:
	$(SWIG) -perl -shadow -c++ $(PREFIX).i
	mv -f $(PREFIX)_wrap.cxx ../perl
	mv -f $(PREFIX).pm ../perl

ruby:
	$(SWIG) -ruby -c++ $(PREFIX).i
	mv -f $(PREFIX)_wrap.cxx ../ruby/$(PREFIX)_wrap.cpp

python:
	$(SWIG) -python -shadow -c++ $(PREFIX).i
	mv -f $(PREFIX)_wrap.cxx ../python
	mv -f $(PREFIX).py ../python

java:
	$(SWIG) -java -package org.chasen.tinysvm -c++ $(PREFIX).i
	mkdir -p ../java/org/chasen/tinysvm
	mv -f *.java ../java/org/chasen/tinysvm
	mv -f $(PREFIX)_wrap.cxx ../java
