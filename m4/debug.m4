AC_DEFUN([AC_TDD_DEBUG],
[
    AC_ARG_ENABLE(debug,
        AS_HELP_STRING([--enable-debug], [enable debug flags]),
        [use_debug=yes], [use_debug=no])

    # Add the special gcc flags
    DEBUG_CFLAGS="-g3 -fsanitize=address"
    DEBUG_LDFLAGS="-fsanitize=address"
]) # AC_TDD_GCOV
