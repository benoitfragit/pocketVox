AM_COMMON_FLAGS = $(DEPS_CFLAGS) $(COMPILER_FLAGS) \
	    -D_GNU_SOURCE                    \
	    -DPKGDATADIR=\"$(pkgdatadir)/\"  \
	    -DPKGLIBDIR=\"$(pkglibdir)/\"    \
	    -DLOCALEDIR=\""$(localedir)"\" \
	    -Wall -Werror -g3 -O2
	    
INCLUDES = 							\
    -I ${top_srcdir}/src/			\
    @GLIB_CFLAGS@					\
    @GTK3_CFLAGS@					\
    @GSTREAMER_CFLAGS@				\
    @APPINDICATOR_CFLAGS@			\
    @POCKETSPHINX_CFLAGS@			\
    @LIBNOTIFY_CFLAGS@

DEBUG_CFLAGS = @DEBUG_CFLAGS@

AM_CFLAGS = $(AM_COMMON_FLAGS) $(DEBUG_CFLAGS)

include_HEADERS = \
	pocketvox_types.h \
	pocketvox_recognizer.h \
	pocketvox_dictionnary.h \
	pocketvox_indicator.h \
	pocketvox_notifier.h \
	pocketvox_controller.h

lib_LTLIBRARIES = libpocketvox.la

libpocketvox_la_SOURCES = \
	pocketvox_recognizer.c \
	pocketvox_dictionnary.c \
	pocketvox_indicator.c \
	pocketvox_notifier.c \
	pocketvox_controller.c

libpocketvox_la_LDFLAGS = \
	@GLIB_LIBS@ \
	@GTK3_LIBS@ \
	@GSTREAMER_LIBS@ \
	@APPINDICATOR_LIBS@ \
	@LIBNOTIFY_LIBS@ \
	@POCKETSPHINX_LIBS@

bin_PROGRAMS = pocketvox

pocketvox_SOURCES = \
	main.c

pocketvox_LDADD = \
	${top_builddir}/src/libpocketvox.la	\
	@GLIB_LIBS@ \
	@GTK3_LIBS@ \
	@GSTREAMER_LIBS@ \
	@APPINDICATOR_LIBS@ \
	@LIBNOTIFY_LIBS@ \
	@POCKETSPHINX_LIBS@

pocketvox_LDFLAGS = -export-dynamic
pocketvox_CFLAGS = $(DEPS_CFLAGS)
