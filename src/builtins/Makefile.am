lib_LIBRARIES = libbuiltins.a

libbuiltins_a_SOURCES = builtins.c builtins.h \
						builtin_echo.c \
						builtin_true_false.c \
						builtin_exit.c \
						builtin_break.c \
						builtin_continue.c \
						builtin_export.c \
						builtin_cd.c

libbuiltins_a_CPPFLAGS = -I$(top_srcdir)/src
libbuiltins_a_CFLAGS = 	-Wall -Wextra -Werror -std=c99 -pedantic \
				  		$(DEBUG_CFLAGS) \
				  		$(COVERAGE_CFLAGS)
