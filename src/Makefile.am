bin_PROGRAMS = 42sh
42sh_SOURCES = main.c

42sh_CPPFLAGS = -I$(top_srcdir)/src
42sh_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic \
				$(DEBUG_CFLAGS) \
				$(COVERAGE_CFLAGS)
42sh_LDADD = 	$(top_builddir)/src/lexer/liblexer.a \
				$(top_builddir)/src/parser/libparser.a \
				$(top_builddir)/src/ast/libast.a \
				$(top_builddir)/src/io/libio.a \
				$(top_builddir)/src/options/liboptions.a \
			 	$(top_builddir)/src/utils/libutils.a \
				$(top_builddir)/src/error_handling/liberrorhandling.a \
				$(top_builddir)/src/builtins/libbuiltins.a
42sh_LDFLAGS =	$(DEBUG_LDFLAGS) \
				$(COVERAGE_LDFLAGS)


SUBDIRS = 	lexer \
			parser \
			ast \
			io \
			options \
			utils \
			error_handling \
			builtins
