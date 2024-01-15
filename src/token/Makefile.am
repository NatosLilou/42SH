# Name of the libaries
lib_LIBRARIES = libtoken.a

# List of file to compile in libtoken.a
libtoken_a_SOURCES = \
	token.c token.h

libtoken_a_CPPFLAGS = -I$(top_srcdir)/src

libtoken_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

noinst_LIBRARIES = libtoken.a
