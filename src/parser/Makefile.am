# Name of the libaries
lib_LIBRARIES = libparser.a

# List of file to compile in libparser.a
libparser_a_SOURCES =  \
	parser_and_or.c \
	parser_command.c \
	parser_element.c \
	parser_input.c \
	parser_list.c \
	parser_pipeline.c \
	parser_simple_command.c \
	parser.h

libparser_a_CPPFLAGS = -I$(top_srcdir)/src

libparser_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

noinst_LIBRARIES = libparser.a
