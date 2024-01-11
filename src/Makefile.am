bin_PROGRAMS = 42sh
42sh_SOURCES = main.c

42sh_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic
42sh_CPPFLAGS = -I$(top_srcdir)/src

42sh_LDADD = 	$(top_builddir)/src/lexer/liblexer.a \
				$(top_builddir)/src/parser/libparser.a \
				$(top_builddir)/src/ast/libast.a \
				$(top_builddir)/src/io_backend/libio_backend.a \
				$(top_builddir)/src/options/liboptions.a


SUBDIRS = 	lexer \
			parser \
			ast \
			io_backend \
			options

