#!/bin/sh
    aclocal
    intltoolize --force
    autoreconf --force --install -I m4
    gtkdocize || exit 1
    ./configure --enable-introspection=yes $1
