bin_PROGRAMS = 42sh
42sh_SOURCES = src/main.c

42sh_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic
42sh_CPPFLAGS = -I$(top_srcdir)/src

42sh_LDADD = $(top_builddir)/src/lexer/liblexer.a

SUBDIRS = src/lexer
