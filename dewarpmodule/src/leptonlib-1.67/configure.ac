AC_PREREQ([2.50])
AC_INIT([leptonica], [1.67], [dan.bloomberg@gmail.com])
AC_CONFIG_AUX_DIR([config])
AM_CONFIG_HEADER([config_auto.h:config/config.h.in])
AC_CONFIG_SRCDIR([src/adaptmap.c])

AC_CONFIG_MACRO_DIR([m4])
LT_INIT

AM_INIT_AUTOMAKE
AC_LANG(C)

# Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Checks for arguments.
AC_ARG_WITH([zlib], AS_HELP_STRING([--without-zlib], [do not include zlib support]))
AC_ARG_WITH([libpng], AS_HELP_STRING([--without-libpng], [do not include libpng support]))
AC_ARG_WITH([jpeg], AS_HELP_STRING([--without-jpeg], [do not include jpeg support]))
AC_ARG_WITH([giflib], AS_HELP_STRING([--without-giflib], [do not include giflib support]))
AC_ARG_WITH([libtiff], AS_HELP_STRING([--without-libtiff], [do not include libtiff support]))

AC_ARG_ENABLE([programs], AS_HELP_STRING([--disable-programs], [do not build additional programs]))
AM_CONDITIONAL([ENABLE_PROGRAMS], [test "x$enable_programs" != xno])

# Checks for libraries.
AS_IF([test "x$with_zlib" != xno],
  AC_CHECK_LIB([z], [deflate],
    AC_DEFINE([HAVE_LIBZ], 1, [Define to 1 if you have zlib.]) AC_SUBST([ZLIB_LIBS], [-lz]),
    AS_IF([test "x$with_zlib" = xyes], AC_MSG_ERROR([zlib support requested but library not found]))
  )
)

AS_IF([test "x$with_libpng" != xno],
  AC_CHECK_LIB([png], [png_read_png],
    AC_DEFINE([HAVE_LIBPNG], 1, [Define to 1 if you have libpng.]) AC_SUBST([LIBPNG_LIBS], [-lpng]),
    AS_IF([test "x$with_libpng" = xyes], AC_MSG_ERROR([libpng support requested but library not found]))
  )
)

AS_IF([test "x$with_jpeg" != xno],
  AC_CHECK_LIB([jpeg], [jpeg_read_scanlines],
    AC_DEFINE([HAVE_LIBJPEG], 1, [Define to 1 if you have jpeg.]) AC_SUBST([JPEG_LIBS], [-ljpeg]),
    AS_IF([test "x$with_jpeg" = xyes], AC_MSG_ERROR([jpeg support requested but library not found]))
  )
)

AS_IF([test "x$with_giflib" != xno],
  AC_CHECK_LIB([gif], [DGifOpenFileHandle],
    AC_DEFINE([HAVE_LIBGIF], 1, [Define to 1 if you have giflib.]) AC_SUBST([GIFLIB_LIBS], [-lgif]),
    AS_IF([test "x$with_giflib" = xyes], AC_MSG_ERROR([giflib support requested but library not found]))
  )
)

AS_IF([test "x$with_libtiff" != xno],
  AC_CHECK_LIB([tiff], [TIFFOpen],
    AC_DEFINE([HAVE_LIBTIFF], 1, [Define to 1 if you have libtiff.]) AC_SUBST([LIBTIFF_LIBS], [-ltiff]),
    AS_IF([test "x$with_libtiff" = xyes], AC_MSG_ERROR([libtiff support requested but library not found]))
  )
)

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_C_BIGENDIAN

AC_SUBST([APPLE_UNIVERSAL_BUILD], [0])
AC_SUBST([ENDIANNESS], [L_LITTLE_ENDIAN])

case "$ac_cv_c_bigendian" in 
  yes) AC_SUBST([ENDIANNESS], [L_BIG_ENDIAN]) ;;
  universal) AC_SUBST([APPLE_UNIVERSAL_BUILD], [1]) ;;
esac

# Checks for library functions.
AC_CHECK_FUNCS([fmemopen])

AC_CONFIG_FILES([Makefile src/endianness.h src/Makefile prog/Makefile])
AC_OUTPUT
