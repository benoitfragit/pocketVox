m4_define([pocketvox_major], [0])
m4_define([pocketvox_minor], [0])
m4_define([pocketvox_micro], [1])
m4_define([pocketvox_version],
        [pocketvox_major.pocketvox_minor.pocketvox_micro])


AC_CONFIG_MACRO_DIR(m4)

# autoconf
AC_INIT([pocketvox],[pocketvox_version])
AC_CONFIG_MACRO_DIR([build/autotools])

AM_INIT_AUTOMAKE
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AC_PROG_LIBTOOL
AC_CANONICAL_HOST
AM_MAINTAINER_MODE
AC_CONFIG_HEADERS([config.h])

AC_ISC_POSIX
AC_HEADER_STDC
AC_DISABLE_STATIC
AC_PROG_LIBTOOL
AC_PROG_CC
AM_PROG_CC_C_O
AM_PATH_GLIB_2_0
PKG_PROG_PKG_CONFIG([0.16])

POCKETVOX_MAJOR_VERSION=pocketvox_major
POCKETVOX_MINOR_VERSION=pocketvox_minor
POCKETVOX_MICRO_VERSION=pocketvox_micro
POCKETVOX_VERSION=pocketvox_version
AC_SUBST(POCKETVOX_MAJOR_VERSION)
AC_SUBST(POCKETVOX_MINOR_VERSION)
AC_SUBST(POCKETVOX_MICRO_VERSION)
AC_SUBST(POCKETVOX_VERSION)

AC_DEFINE_UNQUOTED(POCKETVOX_PREFIX, "${prefix}", [prefix])
PREFIX=${prefix}
AC_SUBST(PREFIX)

LT_PREREQ([2.2.6])
LT_INIT([disable-static])

AC_ARG_ENABLE([debug],
    AS_HELP_STRING([--enable-debug],[Set debug CFLAGS]),,
    enable_debug=$profile_enable_debug)

AC_MSG_CHECKING(for debug flags)
if test "x$enable_debug" = "xyes"; then
    DEBUG_CFLAGS="-O0 -g3 -fno-strict-aliasing"
    dnl -finstrument-functions"
    AC_DEFINE(ENABLE_DEBUG, 1, [Debug CFLAGS on])
    AC_MSG_RESULT(debug on)
else
    DEBUG_CFLAGS="-fno-strict-aliasing"
    AC_MSG_RESULT(debug off)
fi

PKG_CONFIG_PATH+=":/usr/local/lib/pkgconfig:/usr/lib/i386-linux-gnu/pkgconfig"

PKG_CHECK_MODULES(GLIB, [glib-2.0])
AC_MSG_RESULT([GLIB_CFLAGS=$GLIB_CFLAGS])
AC_MSG_RESULT([GLIB_LIBS=$GLIB_LIBS])

AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

PKG_CHECK_MODULES(GSTREAMER, [gstreamer-0.10])
AC_MSG_RESULT([GSTREAMER_CFLAGS=$GSTREAMER_CFLAGS])
AC_MSG_RESULT([GSTREAMER_LIBS=$GSTREAMER_LIBS])

AC_SUBST(GSTREAMER_CFLAGS)
AC_SUBST(GSTREAMER_LIBS)

PKG_CHECK_MODULES(GTK3, [gtk+-3.0])
AC_MSG_RESULT([GTK3_CFLAGS=$GTK3_CFLAGS])
AC_MSG_RESULT([GTK3_LIBS=$GTK3_LIBS])

AC_SUBST(GTK3_CFLAGS)
AC_SUBST(GTK3_LIBS)

PKG_CHECK_MODULES(GDK, [gdk-3.0])
AC_MSG_RESULT([GDK_CFLAGS=$GDK_CFLAGS])
AC_MSG_RESULT([GDK_LIBS=$GDK_LIBS])

AC_SUBST(GDK_CFLAGS)
AC_SUBST(GDK_LIBS)

PKG_CHECK_MODULES(POCKETSPHINX, [pocketsphinx])
AC_MSG_RESULT([POCKETSPHINX_CFLAGS=$POCKETSPHINX_CFLAGS])
AC_MSG_RESULT([POCKETSPHINX_LIBS=$POCKETSPHINX_LIBS])

AC_SUBST(POCKETSPHINX_CFLAGS)
AC_SUBST(POCKETSPHINX_LIBS)

PKG_CHECK_MODULES(LIBNOTIFY, [libnotify])
AC_MSG_RESULT([LIBNOTIFY_CFLAGS=$LIBNOTIFY_CFLAGS])
AC_MSG_RESULT([LIBNOTIFY_LIBS=$LIBNOTIFY_LIBS])

AC_SUBST(LIBNOTIFY_CFLAGS)
AC_SUBST(LIBNOTIFY_LIBS)

PKG_CHECK_MODULES(APPINDICATOR, [appindicator3-0.1])
AC_MSG_RESULT([APPINDICATOR_CFLAGS=$APPINDICATOR_CFLAGS])
AC_MSG_RESULT([APPINDICATOR_LIBS=$APPINDICATOR_LIBS])

AC_SUBST(APPINDICATOR_CFLAGS)
AC_SUBST(APPINDICATOR_LIBS)

PKG_CHECK_MODULES([GOBJECT], [gobject-2.0])
AC_MSG_RESULT([GOBJECT_CFLAGS=$GOBJECT_CFLAGS])
AC_MSG_RESULT([GOBJECT_LIBS=$GOBJECT_LIBS])

AC_SUBST(GOBJECT_CFLAGS)
AC_SUBST(GOBJECT_LIBS)

dnl
dnl Test espeak lib
dnl
AC_CHECK_LIB([espeak], [espeak_Initialize])
AS_IF([test "$ac_cv_lib_espeak_espeak_Initialize" = yes], [HAVE_LIBESPEAK=1], [HAVE_LIBESPEAK=0])
AM_CONDITIONAL([HAVE_LIBESPEAK], [test "$ac_cv_lib_espeak_espeak_Initialize" = yes])


dnl
dnl Add Extra CFLAGS
dnl
WARNING_CFLAGS=" -Wall  -Werror  -Wshadow \
                -Wno-uninitialized -Wno-strict-aliasing \
                -Wformat -Wformat-security \
                -Winit-self  -Wpointer-arith"

DEBUG_CFLAGS+=$WARNING_CFLAGS

dnl
dnl Finely set DEBUG_CFLAGS
dnl
AC_SUBST(DEBUG_CFLAGS)

dnl
dnl GSETTINGS
dnl
GLIB_GSETTINGS

dnl
dnl locale
dnl
IT_PROG_INTLTOOL([0.35.0])

GETTEXT_PACKAGE=Pocketvox
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"],
                   [Domaine de portée de gettext])
AM_GLIB_GNU_GETTEXT

PROGRAMNAME_LOCALEDIR=[${datadir}/locale]
AC_SUBST(PROGRAMNAME_LOCALEDIR)

dnl
dnl Introspection
dnl

GOBJECT_INTROSPECTION_CHECK([>1.32])

# check for gtk-doc
m4_ifdef([GTK_DOC_CHECK], [
GTK_DOC_CHECK([1.18],[--flavour no-tmpl])
],[
AM_CONDITIONAL([ENABLE_GTK_DOC], false)
])

AC_CONFIG_FILES([
                Makefile
                data/Makefile
                src/Makefile
                src/lib/Makefile
                src/gui/Makefile
                po/Makefile.in
                docs/Makefile
                docs/reference/Makefile
                pocketvox.pc
])

AC_CONFIG_FILES([data/pocketvox-launcher.sh], [chmod +x data/pocketvox-launcher.sh])

AC_OUTPUT
