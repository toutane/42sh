TESTS = test_io_backend

check_PROGRAMS = test_io_backend
test_io_backend_SOURCES = test_io_backend.c

# set correct compile flags
test_io_backend_CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla
test_io_backend_LDFLAGS = -lcriterion

# link library to test
test_io_backend_LDADD = $(top_builddir)/src/io_backend/libio_backend.a
