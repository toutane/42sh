SUBDIRS = src \
		  tests

# Clang format
clang:
	clang-format -n $$(find $(top_srcdir) -name *.[ch])
	clang-format -i $$(find $(top_srcdir) -name *.[ch])

# Coverage targets
if HAVE_GCOV

.PHONY: clean-gcda
clean-gcda:
	@echo Removing old coverage results
	-find -name '*.gcda' -print | xargs -r rm

.PHONY: coverage-html generate-coverage-html clean-coverage-html
coverage-html: clean-gcda
	-$(MAKE) $(AM_MAKEFLAGS) -k check
	$(MAKE) $(AM_MAKEFLAGS) generate-coverage-html

generate-coverage-html:
	@echo Collecting coverage data
	$(LCOV) --directory $(top_builddir) --capture --output-file coverage.info --no-checksum --compat-libtool
	LANG=C $(GENHTML) --prefix $(top_builddir) --output-directory coveragereport --title "Code Coverage" --legend --show-details coverage.info

clean-coverage-html: clean-gcda
	-$(LCOV) --directory $(top_builddir) -z
	-rm -rf coverage.info coveragereport

clean-local: clean-coverage-html

endif # HAVE_GCOV
