AC_INIT([42sh], [1.0], [lucas.siauve@epita.fr])
AM_INIT_AUTOMAKE([subdir-objects] [foreign])

AC_PROG_CC
AX_COMPILER_FLAGS([], [], [], [-Wall -Wextra -Werror -std=c99 -pedantic])

AM_PROG_AR
AC_PROG_RANLIB

AC_CONFIG_FILES([
   Makefile
   src/Makefile
   src/lexer/Makefile
   src/parser/Makefile
   src/ast/Makefile
   src/io_backend/Makefile
   src/options/Makefile

   tests/Makefile
   tests/functionals-tests/Makefile
   tests/ast/Makefile
])

AC_OUTPUT
