lib_LIBRARIES = libbuiltins.a

libbuiltins_a_SOURCES = builtins.c builtin_echo.c builtins.h

libbuiltins_a_CPPFLAGS = -I$(top_srcdir)/src
libbuiltins_a_CFLAGS = 	-Wall -Wextra -Werror -std=c99 -pedantic \
				  		$(DEBUG_CFLAGS) \
				  		$(COVERAGE_CFLAGS)
