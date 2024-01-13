bin_PROGRAMS = \
	test_lexer

SUBDIRS = ../../src/lexer

test_lexer_SOURCES = test_lexer.c
test_lexer_CPPFLAGS = -I$(top_srcdir)/src/lexer
test_lexer_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic
test_lexer_LDADD = \
	$(top_srcdir)/src/lexer/liblexer.a

check-local: $(bin_PROGRAMS)
	./testsuite.sh
