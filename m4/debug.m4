AC_DEFUN([AC_TDD_DEBUG],
[
    AC_ARG_ENABLE(debug,
        AS_HELP_STRING([--enable-debug], [enable debug flags]),
        [use_debug=yes], [use_debug=no])
    # Remove all optimization flags from CFLAGS
    changequote({,})
    CFLAGS=`echo "$CFLAGS" | $SED -e 's/-O[0-9]*//g' | $SED -e 's/-g//g'`
    changequote([,])

    # Add the special gcc flags
    DEBUG_CFLAGS="-O0 -g3 -fsanitize=address"
    DEBUG_LDFLAGS="-fsanitize=address"
]) # AC_TDD_GCOV
