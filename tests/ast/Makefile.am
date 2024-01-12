TESTS = test_ast

check_PROGRAMS = test_ast
test_ast_SOURCES = test_ast.c

# Se correct compile flags
test_ast_CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla
test_ast_LDFLAGS = -lcriterion

# link library to test
test_ast_LDADD = $(top_builddir)/src/ast/libast.a
