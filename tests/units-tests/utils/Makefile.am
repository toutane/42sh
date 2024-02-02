TESTS = test_utils

check_PROGRAMS = test_utils
test_utils_SOURCES = test_utils.c

# set correct compile flags
test_utils_CPPFLAGS = -I$(top_srcdir)/src
test_utils_CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla
test_utils_LDFLAGS = -lcriterion

# link library to test
test_utils_LDADD = $(top_builddir)/src/utils/libutils.a
