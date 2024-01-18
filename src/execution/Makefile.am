# Name of the libaries

SUBDIRS = ../builtin

lib_LIBRARIES = libexecution.a

# List of file to compile in libexecution.a
libexecution_a_SOURCES = \
	execution_builtin.c \
	execution_simple_command.c \
	execution_vp.c \
	execution_pipeline.c \
	execution.h

libexecution_a_CPPFLAGS = -I$(top_srcdir)/src

libexecution_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

noinst_LIBRARIES = libexecution.a
