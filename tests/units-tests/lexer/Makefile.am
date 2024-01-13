TESTS = test_lexer

check_PROGRAMS = test_lexer
test_lexer_SOURCES = test_lexer.c

# set correct compile flags
test_lexer_CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla
test_lexer_LDFLAGS = -lcriterion

# link library to test
test_lexer_LDADD = $(top_builddir)/src/lexer/liblexer.a
