AC_DEFUN([AC_TDD_GCOV],
[
    AC_ARG_ENABLE(gcov,
        AS_HELP_STRING([--enable-gcov], [enable coverage testing with gcov]),
        [use_gcov=yes], [use_gcov=no])

    AM_CONDITIONAL(HAVE_GCOV, test "x$use_gcov" = "xyes")

    if test "x$use_gcov" = "xyes"; then
        AC_CHECK_PROG(LCOV, lcov, lcov)
        AC_CHECK_PROG(GENHTML, genhtml, genhtml)

        if test -z "$GENHTML"; then
          AC_MSG_ERROR([Could not find genhtml from the lcov package])
        fi

        # Remove all optimization flags from CFLAGS
        changequote({,})
        CFLAGS=`echo "$CFLAGS" | $SED -e 's/-O[0-9]*//g'`
        changequote([,])

        # Add the special gcc flags
        COVERAGE_CFLAGS="--coverage -fPIC"
        COVERAGE_LDFLAGS="-lgcov"
    fi
]) # AC_TDD_GCOV
