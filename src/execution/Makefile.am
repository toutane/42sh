lib_LIBRARIES = libexecution.a

libexecution_a_SOURCES = exec.c exec.h exec_simple_command.c exec_condition.c exec_list.c

libexecution_a_CPPFLAGS = -I$(top_srcdir)/src
libexecution_a_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic \
				  		$(DEBUG_CFLAGS) \
				  		$(COVERAGE_CFLAGS)
