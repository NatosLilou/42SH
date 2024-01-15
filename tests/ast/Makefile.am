bin_PROGRAMS = \
	test_ast

SUBDIRS = ../../src/parser

test_ast_SOURCES = test_ast.c
test_ast_CPPFLAGS = -I$(top_srcdir)/src/parser
test_ast_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic
test_ast_LDADD = \
	$(top_builddir)/src/parser/libparser.a \
	$(top_builddir)/src/ast/libast.a \
	$(top_builddir)/src/lexer/liblexer.a \
	$(top_builddir)/src/token/libtoken.a

check-local: $(bin_PROGRAMS)
	./testsuite.sh
