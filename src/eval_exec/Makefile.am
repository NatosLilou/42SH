# Name of the libaries

SUBDIRS = \
	../ast \
	../builtin

lib_LIBRARIES = libeval_exec.a

# List of file to compile in libeval_exec.a
libeval_exec_a_SOURCES = \
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
	eval.h \
	execution_builtin.c \
	execution_simple_command.c \
	execution_vp.c \
	execution_pipeline.c \
	execution.h

libeval_exec_a_CPPFLAGS = -I$(top_srcdir)/src

libeval_exec_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

noinst_LIBRARIES = libeval_exec.a
