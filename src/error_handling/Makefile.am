lib_LIBRARIES = liberrorhandling.a

liberrorhandling_a_SOURCES = error_handling.c error_handling.h \
                              error_handling_execvp.c error_handling_execvp.h

liberrorhandling_a_CPPFLAGS = -I$(top_srcdir)/src
liberrorhandling_a_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic \
				  			$(DEBUG_CFLAGS) \
				  			$(COVERAGE_CFLAGS)