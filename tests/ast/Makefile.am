bin_PROGRAMS = \
	test_ast

SUBDIRS = ../../src/parser

test_ast_SOURCES = test_ast.c
test_ast_CPPFLAGS = -I$(top_srcdir)/src/parser
test_ast_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic
test_ast_LDADD = \
	$(top_srcdir)/src/parser/libparser.a

check-local: $(bin_PROGRAMS)
	./testsuite.sh
