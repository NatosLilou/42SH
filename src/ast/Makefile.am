# Name of the libaries
lib_LIBRARIES = libast.a

# List of file to compile in libast.a
libast_a_SOURCES =  \
	ast_and_or.c \
	ast_command.c \
	ast_input.c \
	ast_list.c \
	ast_pipeline.c \
	ast_simple_command.c \
	ast.h

libast_a_CPPFLAGS = -I$(top_srcdir)/src

libast_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

noinst_LIBRARIES = libast.a
