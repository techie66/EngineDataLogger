#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([EngineDataLogger], [v0.2.0], [jacob@geigle.me])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE(1.9 subdir-objects)
AM_MAINTAINER_MODE()

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

AX_CXX_COMPILE_STDCXX_11
# Checks for libraries. Based on enabled features.
AC_ARG_ENABLE([can],
	[AS_HELP_STRING([--disable-can],
		[Disable support for CAN Bus])],
	[],
	[enable_can=yes])

AS_IF([test "x$enable_can" != xno],
	[
   canbus=true
   AC_CHECK_HEADERS([net/if.h linux/can.h linux/can/raw.h ],,[canbus=false])
])
AS_IF([test "x$canbus" = "xtrue"],
  [
    AC_DEFINE([FEAT_CAN],[1],[Support CAN bus])
  ])

AM_CONDITIONAL([CANBUS], [test "x$canbus" = xtrue])

AC_ARG_ENABLE([powercalc],
	[AS_HELP_STRING([--disable-powercalc],
		[Disable support for calculating power from weight/speed/and accel])],
	[],
	[enable_powercalc=yes])

AS_IF([test "x$enable_powercalc" != xno],
	[
   powercalc=true
   #AC_CHECK_HEADERS([ ],,[powercalc=false])
])
AS_IF([test "x$powercalc" = "xtrue"],
  [
    AC_DEFINE([FEAT_POWERCALC],[1],[Support Power Calculation])
  ])

AM_CONDITIONAL([POWERCALC], [test "x$powercalc" = xtrue])

AC_ARG_ENABLE([frontcontrols],
	[AS_HELP_STRING([--disable-frontcontrols],
		[Disable support for Front Controls])],
	[],
	[enable_frontcontrols=yes])
AS_IF([test "x$enable_frontcontrols" != xno],
	[AC_DEFINE([FEAT_FRONTCONTROLS],[1],[Support Front Controls])
	front_controls=true
])
AM_CONDITIONAL([FRONTCONTROLS], [test "x$front_controls" = xtrue])

AC_ARG_ENABLE([i2c],
	[AS_HELP_STRING([--disable-i2c],
		[Disable support for I2C])],
	[],
	[enable_i2c=yes])
# TODO check for i2c headers?
AS_IF([test "x$enable_i2c" != xno],
	[AC_DEFINE([FEAT_I2C],[1],[Support I2C])
	feat_i2c=true
])
AM_CONDITIONAL([I2C], [test "x$feat_i2c" = xtrue])

AC_ARG_ENABLE([gpio],
	[AS_HELP_STRING([--disable-gpio],
		[Disable support for GPIO])],
	[],
	[enable_gpio=yes])
AC_ARG_WITH([bcm2835],
	[AS_HELP_STRING([--without-bcm2835],
		[compile without bcm2835, which is needed for gpio such as O2 sensor power control.])],
		[],
		[with_bcm2835=yes])
AS_IF([test "x$with_bcm2835" != xno -a "x$enable_gpio" != xno],
	[AC_CHECK_LIB([bcm2835], [bcm2835_i2c_write_read_rs],,[
		AC_MSG_ERROR([libbcm2835 NOT FOUND! You can try automatic installation with: `./install-deps.sh bcm2835`])
	])
])
AM_CONDITIONAL([GPIO], [test "$ac_cv_lib_bcm2835_bcm2835_i2c_write_read_rs" = "yes"])

AC_ARG_WITH([ignitech],
	[AS_HELP_STRING([--without-ignitech],
		[Compile without libIgnitech, which is needed to communicate with an Ignitech TCIP-4 ignition.])],
	[],
	[with_ignitech=yes])
AC_ARG_ENABLE([ignitech],
	[AS_HELP_STRING([--disable-ignitech],
		[Disable support for Ignitech Ignition])],
	[],
	[enable_ignitech=yes])
AS_IF([test "x$with_ignitech" != xno -a "x$enable_ignitech" != xno],
	[AC_CHECK_LIB([ignitech], [main],,[
		AC_MSG_ERROR([libIgnitech NOT FOUND! You can try automatic installation with: `./install-deps.sh ignitech`])
	])
])
AM_CONDITIONAL([IGNITECH], [test "$ac_cv_lib_ignitech_main" = "yes"])

AC_ARG_WITH([isp2],
	[AS_HELP_STRING([--without-isp2],
		[Compile without libISP2, which is needed to communicate with an Innovate LC-2 lambda controller.])],
	[],
	[with_isp2=yes])
AC_ARG_ENABLE([isp2],
	[AS_HELP_STRING([--disable-isp2],
		[Disable support for Innovate LC-2 Lambda controller.])],
	[],
	[enable_isp2=yes])
AS_IF([test "x$with_isp2" != xno -a "x$enable_isp2" != xno],
	[AC_CHECK_LIB([isp2], [main],,
		[AC_MSG_ERROR([libISP NOT FOUND! You can try automatic installation with: ./install-deps.sh isp])
	])
])
AM_CONDITIONAL([ISP2],[test "$ac_cv_lib_isp2_main" = "yes"])

AC_ARG_WITH([bluetooth],
	[AS_HELP_STRING([--without-bluetooth],
		[Compile without bluetooth, which is needed to communicate with the Android dashboard.])],
	[],
	[with_bluetooth=yes])
AC_ARG_WITH([flatbuffers],
	[AS_HELP_STRING([--without-flatbuffers],
		[Compile without Flatbuffers, which is needed to communicate with the Android dashboard.])],
	[],
	[with_flatbuffers=yes])
AC_ARG_ENABLE([dashboard],
	[AS_HELP_STRING([--disable-dashboard],
		[Disable support for the Android dashboard.])],
	[],
	[enable_dashboard=yes])

AS_IF([test "x$with_bluetooth" != xno -a "x$with_flatbuffers" != xno -a "x$enable_dashboard" != xno],
	[AC_CHECK_LIB([bluetooth], [ba2str],,[
		AC_MSG_ERROR([bluetooth support NOT FOUND! You can try automatic installation with: ./install-deps.sh bluetooth])
	])
	AC_CHECK_LIB([flatbuffers], [main],,[
		AC_MSG_ERROR([flatbuffers NOT FOUND! You can try automatic installation with: ./install-deps.sh flatbuffers])
	])
  AC_DEFINE([FEAT_DASHBOARD],[1],[Support Android dashboard])
 	feat_dashboard=true
])
AM_CONDITIONAL([DASHBOARD], [test "x$feat_dashboard" = xtrue])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h sys/ioctl.h sys/socket.h sys/time.h termios.h unistd.h float.h math.h errno.h stddef.h],,[
  AC_MSG_ERROR([needed headers not found])                
  ])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T
AC_TYPE_SIZE_T
AC_TYPE_INT16_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([gettimeofday memset select socket strdup strerror strchr strcspn strspn strtol strndup],,
  AC_MSG_ERROR([library functions not found]))

# Check for utility progs
AC_PROG_LN_S
AC_PROG_RANLIB

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

AC_CHECK_PROG([HAVE_GENGETOPT],[gengetopt],[yes])
AM_CONDITIONAL([HAVE_GENGETOPT], [test "x$HAVE_GENGETOPT" = xyes])

AC_CONFIG_FILES([Makefile data/cmdline.ggo])
AC_OUTPUT
