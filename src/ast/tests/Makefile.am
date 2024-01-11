bin_PROGRAMS = test_ast
test_ast_SOURCES = test_ast.c ../ast.c

test_ast_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic \
					-g3 -fsanitize=address
test_ast_CPPFLAGS = -I$(top_srcdir)/src/ast
test_ast_LDFLAGS = -lcriterion -fsanitize=address
