lib_LIBRARIES = libexpansion.a

libexpansion_a_SOURCES = expansion.c expansion.h \
						 special_variables.c \
						 special_variables.h \
						 HOME_variable.c \
						 CDPATH_variable.c

libexpansion_a_CPPFLAGS = -I$(top_srcdir)/src
libexpansion_a_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic \
				  	$(DEBUG_CFLAGS) \
				  	$(COVERAGE_CFLAGS)
