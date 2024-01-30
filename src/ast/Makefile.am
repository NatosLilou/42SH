# Name of the libaries
lib_LIBRARIES = libast.a

SUBDIRS = \
	../token

# List of file to compile in libast.a
libast_a_SOURCES = \
	ast_and_or.c \
	ast_case_item.c \
	ast_case_clause.c \
	ast_command.c \
	ast_compound_list.c \
	ast_else_clause.c \
	ast_funcdec.c \
	ast_input.c \
	ast_list.c \
	ast_pipeline.c \
	ast_rule_if.c \
	ast_rule_while.c \
	ast_rule_until.c \
	ast_rule_for.c \
	ast_rule_case.c \
	ast_command.c \
	ast_shell_command.c \
	ast_simple_command.c \
	ast_prefix.c \
	ast_redir.c \
	ast.h

libast_a_CPPFLAGS = -I$(top_srcdir)/src

libast_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

noinst_LIBRARIES = libast.a
