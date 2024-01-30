lib_LIBRARIES = libexecution.a

libexecution_a_SOURCES = exec.c exec.h \
						 exec_simple_command.c \
						 exec_condition.c \
						 exec_list.c \
						 exec_pipeline.c \
						 exec_while.c \
						 exec_until.c \
						 exec_for.c \
						 exec_redirections.c \
						 exec_neg.c \
						 exec_and_or.c \
						 break_continue_number.c \
						 exec_subshell.c \
						 exec_func.c

libexecution_a_CPPFLAGS = -I$(top_srcdir)/src
libexecution_a_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic \
				  		$(DEBUG_CFLAGS) \
				  		$(COVERAGE_CFLAGS)
