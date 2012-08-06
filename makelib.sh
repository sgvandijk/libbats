#!/bin/bash

find . -name '*.hh' | egrep -v '(_sse)|(_fpu)' | perl -ne 'chomp; print "#include \""; print; print "\"\n";' > libbats.cc
ccbuild build --compiler 'libtool --tag=CXX --mode=compile g++' libbats.cc --verbose --args "-O2 -DBATS_NODEBUG -DNODEBUG `xml2-config --cflags`"
find . -name '*.lo' -exec libtool --tag=CXX --mode=link g++ -g -O0 -o libbats.la -rpath /usr/local/lib -lxml2 {} +

