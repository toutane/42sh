TESTS = test_parser

check_PROGRAMS = test_parser
test_parser_SOURCES = test_parser.c

# set correct compile flags
test_parser_CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla
test_parser_LDFLAGS = -lcriterion

# link library to test
test_parser_LDADD = $(top_builddir)/src/parser/libparser.a
