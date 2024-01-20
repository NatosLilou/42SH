# Name of the libaries
lib_LIBRARIES = libexpansion.a

SUBDIRS = ../lexer

# List of file to compile in liblexer.a
libexpansion_a_SOURCES = \
	expansion.c expansion.h

libexpansion_a_CPPFLAGS = -I$(top_srcdir)/src

libexpansion_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

noinst_LIBRARIES = libexpansion.a
