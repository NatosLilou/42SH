# define the sub directories
SUBDIRS = \
	parser \
	eval \
	io_back_end

bin_PROGRAMS = 42sh

42sh_SOURCES = 42sh.c

42sh_CPPFLAGS = \
	-I$(top_srcdir)/src/eval \
	-I$(top_srcdir)/src/io_back_end \
	-I$(top_srcdir)/src/parser

42sh_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

42sh_LDADD =  \
	$(top_srcdir)/src/eval/libeval.a \
	$(top_srcdir)/src/io_back_end/libio_back_end.a \
	$(top_srcdir)/src/parser/libparser.a
