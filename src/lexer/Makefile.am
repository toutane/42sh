lib_LIBRARIES = liblexer.a

liblexer_a_SOURCES = lexer.c lexer.h
liblexer_a_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic
liblexer_a_CPPFLAGS = -I$(top_srcdir)/src

bin_PROGRAMS = test_lexer
test_lexer_SOURCES = tests/test_lexer.c lexer.c

test_lexer_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -g3
test_lexer_CPPFLAGS = -I$(top_srcdir)/src/lexer
test_lexer_LDFLAGS = -lcriterion
