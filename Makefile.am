ACLOCAL_AMFLAGS = -I build/autotools
DISTCHECK_CONFIGURE_FLAGS = --enable-introspection
SUBDIRS = data src docs po

INTLTOOL_FILES = intltool-extract.in \
                 intltool-merge.in \
                 intltool-update.in

EXTRA_DIST=$(INTLTOOL_FILES)

pkgconfig_DATA = pocketvox.pc
pkgconfigdir   = $(libdir)/pkgconfig

DISTCLEANFILES = ... intltool-extract \
                 intltool-merge \
                 intltool-update \
                 po/.intltool-merge-cache
