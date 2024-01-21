lib_LIBRARIES = libutils.a

libutils_a_SOURCES =	dot_file/dot_file.c dot_file/dot_file.h \
						token/token.c token/token.h \
						printers/printers.c printers/printers.h \
						strings/strings.c strings/strings.h \
						memory/memory.c memory/memory.h \
						hash_map/hash_map.c hash_map/hash_map.h \
						hash_map/hash_map_setup.c hash_map/hash_map_setup.h \
						assignment_words/assignment_words.c assignment_words/assignment_words.h \
						variables/variables.c variables/variables.h

libutils_a_CPPFLAGS = -I$(top_srcdir)/src
libutils_a_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic
					  $(DEBUG_CFLAGS) \
					  $(COVERAGE_CFLAGS)
