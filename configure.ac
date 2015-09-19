AC_INIT([btrfs-register], [0.1])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_SRCDIR([src/btrfs-register.c])
AM_INIT_AUTOMAKE([-Wall foreign subdir-objects no-dependencies])

AC_PROG_CC
AC_CHECK_HEADER([linux/btrfs.h], [], [AC_MSG_ERROR([Couldn't find btrfs.h])])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
