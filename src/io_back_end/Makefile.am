# Name of the libaries
lib_LIBRARIES = libio_back_end.a

# List of file to compile in libio_back_end.a
libio_back_end_a_SOURCES =  \
	io_back_end.c io_back_end.h

libio_back_end_a_CPPFLAGS = -I$(top_srcdir)/src

libio_back_end_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

noinst_LIBRARIES = libio_back_end.a
