# define the sub directories
SUBDIRS = \
	ast \
	builtin \
	io_back_end \
	lexer \
	parser \
	token

bin_PROGRAMS = 42sh

42sh_SOURCES = 42sh.c

42sh_CPPFLAGS = -I$(top_srcdir)/src

42sh_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

42sh_LDADD =  \
	$(top_srcdir)/src/ast/libast.a \
	$(top_srcdir)/src/builtin/libbuiltin.a \
	$(top_srcdir)/src/io_back_end/libio_back_end.a \
	$(top_srcdir)/src/lexer/liblexer.a \
	$(top_srcdir)/src/parser/libparser.a
	$(top_srcdir)/src/token/libtoken.a
