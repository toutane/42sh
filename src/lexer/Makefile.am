lib_LIBRARIES = liblexer.a

liblexer_a_SOURCES = lexer.c lexer.h \
					 lexer_setup.c lexer_setup.h \
					 lexer_error.c lexer_error.h \
					 token_recognition.c token_recognition_quoting.c token_recognition.h \
					 token_recognition_helpers.c token_recognition_helpers.h

liblexer_a_CPPFLAGS = -I$(top_srcdir)/src
liblexer_a_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic \
				  	$(DEBUG_CFLAGS) \
				  	$(COVERAGE_CFLAGS)
