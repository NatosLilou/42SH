# define the sub directories
SUBDIRS = \
	eval_exec \
	io_back_end \
	parser

bin_PROGRAMS = 42sh

42sh_SOURCES = 42sh.c

42sh_CPPFLAGS = \
	-I$(top_srcdir)/src/eval_exec \
	-I$(top_srcdir)/src/io_back_end \
	-I$(top_srcdir)/src/parser

42sh_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

42sh_LDADD =  \
	$(top_builddir)/src/io_back_end/libio_back_end.a \
	$(top_builddir)/src/parser/libparser.a \
	$(top_builddir)/src/eval_exec/libeval_exec.a \
	$(top_builddir)/src/builtin/libbuiltin.a \
	$(top_builddir)/src/expansion/libexpansion.a \
	$(top_builddir)/src/ast/libast.a \
	$(top_builddir)/src/lexer/liblexer.a \
	$(top_builddir)/src/token/libtoken.a
