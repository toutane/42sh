bin_PROGRAMS = test_lexer
test_lexer_SOURCES = test_lexer.c ../lexer.c

test_lexer_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic \
					-g3 -fsanitize=address
test_lexer_CPPFLAGS = -I$(top_srcdir)/src/lexer
test_lexer_LDFLAGS = -lcriterion -fsanitize=address
