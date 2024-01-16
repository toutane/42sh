lib_LIBRARIES = liblexer.a

liblexer_a_SOURCES = lexer.c lexer.h lexer_setup.c expansion.c

liblexer_a_CPPFLAGS = -I$(top_srcdir)/src
liblexer_a_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic \
				  	$(DEBUG_CFLAGS) \
				  	$(COVERAGE_CFLAGS)
