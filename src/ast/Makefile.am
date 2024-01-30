lib_LIBRARIES = libast.a

libast_a_SOURCES = ast.c ast.h \
					node_func/if_node.c \
					node_func/list_node.c \
					node_func/pipeline_node.c \
					node_func/redirection_node.c \
					node_func/sc_node.c \
					node_func/for_node.c \
					node_func/func_node.c

libast_a_CPPFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/src/ast
libast_a_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic \
				  $(DEBUG_CFLAGS) \
				  $(COVERAGE_CFLAGS)
