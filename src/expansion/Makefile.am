lib_LIBRARIES = libexpansion.a

libexpansion_a_SOURCES = quote_expansion.c quote_expansion.h \
					 	  param_expansion.c param_expansion.h

libexpansion_a_CPPFLAGS = -I$(top_srcdir)/src
libexpansion_a_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic \
				  	$(DEBUG_CFLAGS) \
				  	$(COVERAGE_CFLAGS)
