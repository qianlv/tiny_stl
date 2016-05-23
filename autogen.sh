#!/bin/sh

rm -f config.cache config.log

aclocal \
&& \
autoheader \
&& \
automake --add-missing -Wno-portability \
&& \
autoconf \
&& \
./configure
