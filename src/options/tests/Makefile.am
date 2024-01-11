bin_PROGRAMS = test_options
test_options_SOURCES = test_options.c ../options.c

test_options_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic \
					-g3 -fsanitize=address
test_options_CPPFLAGS = -I$(top_srcdir)/src/options
test_options_LDFLAGS = -lcriterion -fsanitize=address
