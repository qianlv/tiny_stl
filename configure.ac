#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([TinyStl], [0.1], [qianlv7@gmail.com])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
#AC_CONFIG_SRCDIR([test/unit_test.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# 编译参数
CXXFLAGS = "--std=c++11"
AC_SUBST(CXXFLAGS)

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC

AC_CONFIG_FILES([Makefile test/Makefile])
AC_OUTPUT
