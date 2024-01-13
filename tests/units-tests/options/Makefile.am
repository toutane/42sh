TESTS = test_options

check_PROGRAMS = test_options
test_options_SOURCES = test_options.c

# set correct compile flags
test_options_CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla
test_options_LDFLAGS = -lcriterion

# link library to test
test_options_LDADD = $(top_builddir)/src/options/liboptions.a
