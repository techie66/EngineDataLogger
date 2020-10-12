#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([EngineDataLogger], [v0.1], [jacob@geigle.me])
AC_CONFIG_SRCDIR([src/cb1100f-app_generated.h])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE(1.9 subdir-objects)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries. Based on enabled features.

AC_ARG_ENABLE([i2c],
	      [AS_HELP_STRING([--disable-i2c],
			      [disable support for I2C which is needed for communicating with a SleepyPi as part of the "EngineData" module])],
			      [],
			      [enable_i2c=yes])
AS_IF([test "x$enable_i2c" != xno],
	[AC_CHECK_LIB([bcm2835], [bcm2835_i2c_write_read_rs],,[
		      ./install-deps.sh bcm2835
		      LIBS="$LIBS -lbcm2835"
		      ])
	AC_DEFINE([FEAT_I2C],[1],[Support talking to i2c devices])
	])


AC_CHECK_LIB([bluetooth], [ba2str],,[
	      ./install-deps.sh bluetooth
	      LIBS="-lbluetooth $LIBS"
	      ])
AC_CHECK_LIB([ignitech], [main],,[
	      ./install-deps.sh ignitech
	      LIBS="$LIBS -lignitech"
	      ])
AC_CHECK_LIB([isp2], [main],,[
	      ./install-deps.sh isp
	      LIBS="-lisp2 $LIBS"
	      ])
AC_CHECK_LIB([flatbuffers], [main],,[
	      ./install-deps.sh flatbuffers
	      LIBS="-lflatbuffers $LIBS"
	      ])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h sys/ioctl.h sys/socket.h sys/time.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([gettimeofday memset select socket strdup strerror])

PKG_PROG_PKG_CONFIG
AC_ARG_WITH([systemdsystemunitdir],
     [AS_HELP_STRING([--with-systemdsystemunitdir=DIR], [Directory for systemd service files])],,
     [with_systemdsystemunitdir=auto])
AS_IF([test "x$with_systemdsystemunitdir" = "xyes" -o "x$with_systemdsystemunitdir" = "xauto"], [
     def_systemdsystemunitdir=$($PKG_CONFIG --variable=systemdsystemunitdir systemd)

     AS_IF([test "x$def_systemdsystemunitdir" = "x"],
   [AS_IF([test "x$with_systemdsystemunitdir" = "xyes"],
    [AC_MSG_ERROR([systemd support requested but pkg-config unable to query systemd package])])
    with_systemdsystemunitdir=no],
   [with_systemdsystemunitdir="$def_systemdsystemunitdir"])])
AS_IF([test "x$with_systemdsystemunitdir" != "xno"],
      [AC_SUBST([systemdsystemunitdir], [$with_systemdsystemunitdir])])
AM_CONDITIONAL([HAVE_SYSTEMD], [test "x$with_systemdsystemunitdir" != "xno"])


AC_CONFIG_FILES([Makefile])
AC_OUTPUT
