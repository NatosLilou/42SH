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
	parser.h \
	../ast/ast_and_or.c \
	../ast/ast_command.c \
	../ast/ast_input.c \
	../ast/ast_list.c \
	../ast/ast_pipeline.c \
	../ast/ast_simple_command.c \
	../ast/ast.h \
	../lexer/lexer.c ../lexer/lexer.h \
	../token/token.c ../token/token.h


libparser_a_CPPFLAGS = -I$(top_srcdir)/src

libparser_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

noinst_LIBRARIES = libparser.a
