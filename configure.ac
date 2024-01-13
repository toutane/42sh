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
   src/builtins/Makefile
   src/utils/Makefile
   src/error_handling/Makefile

   tests/Makefile
   tests/functionals-tests/Makefile
   tests/units-tests/lexer/Makefile
   tests/units-tests/parser/Makefile
   tests/units-tests/ast/Makefile
   tests/units-tests/io_backend/Makefile
   tests/units-tests/options/Makefile
   tests/units-tests/utils/Makefile
])

AC_OUTPUT
