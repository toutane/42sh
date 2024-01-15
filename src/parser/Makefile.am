lib_LIBRARIES = libparser.a

libparser_a_SOURCES = parser.c parser.h

libparser_a_CPPFLAGS = -I$(top_srcdir)/src
libparser_a_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic \
				  		$(DEBUG_CFLAGS) \
				  		$(COVERAGE_CFLAGS)
