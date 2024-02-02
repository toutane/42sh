lib_LIBRARIES = libparser.a

libparser_a_SOURCES = parser.h parser.c \
					  rule/parser_input.c \
					  rule/parser_list.c \
					  rule/parser_and_or.c \
					  rule/parser_pipeline.c \
					  rule/parser_command.c \
					  rule/parser_simple_command.c \
					  rule/parser_element.c \
					  rule/parser_shell_command.c \
					  rule/parser_rule_if.c \
					  rule/parser_else_clause.c \
					  rule/parser_compound_list.c \
					  rule/parser_rule_for.c \
					  rule/parser_rule_until.c \
					  rule/parser_rule_while.c \
					  rule/parser_redirection.c \
					  rule/parser_prefix.c \
					  rule/parser_fundec.c \
					  rule/parser_rule_case.c

libparser_a_CPPFLAGS = -I$(top_srcdir)/src
libparser_a_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic \
				  		$(DEBUG_CFLAGS) \
				  		$(COVERAGE_CFLAGS)
