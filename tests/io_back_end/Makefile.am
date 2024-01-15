bin_PROGRAMS = \
	test_io_back_end

SUBDIRS = ../../src/io_back_end

test_io_back_end_SOURCES = test_io_back_end.c
test_io_back_end_CPPFLAGS = -I$(top_srcdir)/src/io_back_end
test_io_back_end_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic
test_io_back_end_LDADD = \
	$(top_builddir)/src/io_back_end/libio_back_end.a

check-local: $(bin_PROGRAMS)
	./testsuite.sh
