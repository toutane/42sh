bin_PROGRAMS = test_parser
test_parser_SOURCES = test_parser.c ../parser.c

test_parser_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic \
					-g3 -fsanitize=address
test_parser_CPPFLAGS = -I$(top_srcdir)/src/parser
test_parser_LDFLAGS = -lcriterion -fsanitize=address
