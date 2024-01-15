# Name of the libaries
lib_LIBRARIES = libeval.a

SUBDIRS = \
	../ast \
	../execution

# List of file to compile in libeval.a
libeval_a_SOURCES = \
	eval_and_or.c \
	eval_command.c \
	eval_compound_list.c \
	eval_else_clause.c \
	eval_input.c \
	eval_list.c \
	eval_pipeline.c \
	eval_rule_if.c \
	eval_shell_command.c \
	eval_simple_command.c \
	eval.h

libeval_a_CPPFLAGS = -I$(top_srcdir)/src

libeval_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

noinst_LIBRARIES = libeval.a
