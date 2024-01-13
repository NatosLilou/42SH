# Name of the libaries
lib_LIBRARIES = libexecution.a

# List of file to compile in libexecution.a
libexecution_a_SOURCES =  \
	execution.c execution.h \
	../builtin/builtin.h \
	../builtin/boolean.c \
	../builtin/echo.c

libexecution_a_CPPFLAGS = -I$(top_srcdir)/src

libexecution_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

noinst_LIBRARIES = libexecution.a
