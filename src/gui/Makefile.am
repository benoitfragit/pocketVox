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

gsettings_SCHEMAS = org.pocketvox.gschema.xml
@GSETTINGS_RULES@

bin_PROGRAMS = pocketvox-gtk

pocketvox_gtk_SOURCES = \
	pocketvox-setup.c \
	pocketvox-setup.h \
	main.c
	
pocketvox_gtk_LDADD = \
	${top_builddir}/src/lib/libpocketvox-1.0.la	\
	@GLIB_LIBS@ \
	@GTK3_LIBS@ \
	@GDK_LIBS@ \
	@GSTREAMER_LIBS@ \
	@APPINDICATOR_LIBS@ \
	@LIBNOTIFY_LIBS@ \
	@POCKETSPHINX_LIBS@

pocketvox_gtk_LDFLAGS = -export-dynamic
pocketvox_gtk_CFLAGS = $(DEPS_CFLAGS)
