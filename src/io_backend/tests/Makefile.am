bin_PROGRAMS = test_io_backend
test_io_backend_SOURCES = test_io_backend.c ../io_backend.c

test_io_backend_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic \
					-g3 -fsanitize=address
test_io_backend_CPPFLAGS = -I$(top_srcdir)/src/io_backend
test_io_backend_LDFLAGS = -lcriterion -fsanitize=address
