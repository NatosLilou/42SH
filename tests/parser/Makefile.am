bin_PROGRAMS = \
	test_parser

SUBDIRS = ../../src/parser

test_parser_SOURCES = test_parser.c
test_parser_CPPFLAGS = -I$(top_srcdir)/src/parser
test_parser_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic
test_parser_LDADD = \
	$(top_builddir)/src/parser/libparser.a \
	$(top_builddir)/src/ast/libast.a \
	$(top_builddir)/src/lexer/liblexer.a \
	$(top_builddir)/src/token/libtoken.a

check-local: $(bin_PROGRAMS)
	./test_parser
