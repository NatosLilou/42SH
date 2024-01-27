# Name of the libaries
lib_LIBRARIES = libbuiltin.a

SUBDIRS = \
	../expansion

# List of file to compile in libbuiltin.a
libbuiltin_a_SOURCES = \
	boolean.c \
	echo.c \
	export.c \
	unset.c \
	exit.c \
	builtin.h

libbuiltin_a_CPPFLAGS = -I$(top_srcdir)/src

libbuiltin_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

noinst_LIBRARIES = libbuiltin.a
