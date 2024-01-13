SUBDIRS = src \
		  tests

clang:
	clang-format -n $$(find $(top_srcdir) -name *.[ch])
	clang-format -i $$(find $(top_srcdir) -name *.[ch])

include $(top_srcdir)/Makefile.am.coverage
