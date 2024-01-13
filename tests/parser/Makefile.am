bin_PROGRAMS = \
	test_parser

SUBDIRS = ../../src/parser

test_parser_SOURCES = test_parser.c
test_parser_CPPFLAGS = -I$(top_srcdir)/src/parser
test_parser_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic
test_parser_LDADD = \
	$(top_srcdir)/src/parser/libparser.a

check-local: $(bin_PROGRAMS)
	./test_parser
