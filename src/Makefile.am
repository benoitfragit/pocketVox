AM_COMMON_FLAGS = $(DEPS_CFLAGS) $(COMPILER_FLAGS) \
	    -D_GNU_SOURCE                    \
	    -DPKGDATADIR=\"$(pkgdatadir)/\"  \
	    -DPKGLIBDIR=\"$(pkglibdir)/\"    \
	    -DLOCALEDIR=\""$(localedir)"\" \
	    -Wall -Werror -g3 -O2 -lm -lX11

if HAVE_LIBESPEAK
	AM_COMMON_FLAGS += -lespeak
endif
	    
AM_CPPFLAGS = 						\
    -I ${top_srcdir}/src/			\
    @GLIB_CFLAGS@					\
    @GTK3_CFLAGS@					\
    @GDK_CFLAGS@					\
    @GSTREAMER_CFLAGS@				\
    @APPINDICATOR_CFLAGS@			\
    @POCKETSPHINX_CFLAGS@			\
    @LIBNOTIFY_CFLAGS@ 				\
    @GOBJECT_CFLAGS@ \
	-DPROGRAMNAME_LOCALEDIR=\"${PROGRAMNAME_LOCALEDIR}\"

DEBUG_CFLAGS = @DEBUG_CFLAGS@

AM_CFLAGS = $(AM_COMMON_FLAGS) $(DEBUG_CFLAGS)

include_HEADERS = 			\
	$(top_srcdir)/config.h	\
	pocketvox-types.h 		\
	pocketvox-xmanager.h	\
	pocketvox-profile.h		\
	pocketvox-recognizer.h 	\
	pocketvox-dictionnary.h \
	pocketvox-module.h 		\
	pocketvox-chooser.h		\
	pocketvox-indicator.h 	\
	pocketvox-notifier.h 	\
	pocketvox-controller.h 	\
	pocketvox-application.h

sources_h = 									\
	$(top_srcdir)/src/pocketvox-types.h 		\
	$(top_srcdir)/src/pocketvox-profile.h		\
	$(top_srcdir)/src/pocketvox-xmanager.h		\
	$(top_srcdir)/src/pocketvox-recognizer.h 	\
	$(top_srcdir)/src/pocketvox-dictionnary.h 	\
	$(top_srcdir)/src/pocketvox-module.h 		\
	$(top_srcdir)/src/pocketvox-chooser.h		\
	$(top_srcdir)/src/pocketvox-indicator.h 	\
	$(top_srcdir)/src/pocketvox-notifier.h		\
	$(top_srcdir)/src/pocketvox-controller.h 	\
	$(top_srcdir)/src/pocketvox-application.h

sources_c = \
	pocketvox-profile.c		\
	pocketvox-xmanager.c	\
	pocketvox-recognizer.c 	\
	pocketvox-dictionnary.c \
	pocketvox-chooser.c		\
	pocketvox-indicator.c 	\
	pocketvox-notifier.c 	\
	pocketvox-controller.c 	\
	pocketvox-module.c 		\
	pocketvox-application.c

lib_LTLIBRARIES = libpocketvox-1.0.la

libpocketvox_1_0_la_SOURCES = \
	$(sources_c) \
	$(sources_h)

libpocketvox_1_0_la_LDFLAGS = \
	@GLIB_LIBS@ \
	@GTK3_LIBS@ \
	@GDK_LIBS@ \
	@GSTREAMER_LIBS@ \
	@APPINDICATOR_LIBS@ \
	@LIBNOTIFY_LIBS@ \
	@POCKETSPHINX_LIBS@ \
	@GOBJECT_LIBS@
	
bin_PROGRAMS = pocketvox

pocketvox_SOURCES = \
	main.c

pocketvox_LDADD = \
	${top_builddir}/src/libpocketvox-1.0.la	\
	@GLIB_LIBS@ \
	@GTK3_LIBS@ \
	@GDK_LIBS@ \
	@GSTREAMER_LIBS@ \
	@APPINDICATOR_LIBS@ \
	@LIBNOTIFY_LIBS@ \
	@POCKETSPHINX_LIBS@

pocketvox_LDFLAGS = -export-dynamic
pocketvox_CFLAGS = $(DEPS_CFLAGS)

-include $(INTROSPECTION_MAKEFILE)

# Make sure the name of your library matches the name you defined in
# lib_LTLIBRARIES. Capitalization can be important in these steps.
INTROSPECTION_GIRS = Pocketvox_1_0_gir
# These are flags to pass to g-ir-scanner and g-ir-compiler.
INTROSPECTION_SCANNER_ARGS = --add-include-path=$(srcdir) --warn-all
INTROSPECTION_COMPILER_ARGS =

if HAVE_INTROSPECTION
introspection_sources = $(libpocketvox_1_0_la_SOURCES)

# This step is fairly well documented in the Gnome wiki here:
# https://wiki.gnome.org/Projects/GObjectIntrospection/AutotoolsIntegration
Pocketvox-1.0.gir: libpocketvox-1.0.la
Pocketvox_1_0_gir_INCLUDES = GObject-2.0 GLib-2.0 Gst-0.10 Gtk-3.0 Gdk-3.0 Notify-0.7 AppIndicator3-0.1
Pocketvox_1_0_gir_CFLAGS = $(AM_CPPFLAGS)
Pocketvox_1_0_gir_PACKAGES = gtk+-3.0 gdk-3.0 gobject-2.0 glib-2.0 appindicator3-0.1 libnotify pocketsphinx gstreamer-0.10
Pocketvox_1_0_gir_LIBS = libpocketvox-1.0.la
Pocketvox_1_0_gir_FILES = $(introspection_sources)
Pocketvox_1_0_gir_NAMESPACE = Pocketvox
INTROSPECTION_GIRS += Pocketvox-1.0.gir

girdir = $(datadir)/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES = $(gir_DATA) $(typelib_DATA)
endif
