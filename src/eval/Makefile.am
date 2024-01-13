# Name of the libaries
lib_LIBRARIES = libeval.a

# List of file to compile in libeval.a
libeval_a_SOURCES =  \
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
	../ast/ast_and_or.c \
	../ast/ast_command.c \
	../ast/ast_compound_list.c \
	../ast/ast_else_clause.c \
	../ast/ast_input.c \
	../ast/ast_list.c \
	../ast/ast_pipeline.c \
	../ast/ast_rule_if.c \
	../ast/ast_command.c \
	../ast/ast_shell_command.c \
	../ast/ast_simple_command.c \
	../ast/ast.h \
	../execution/execution_builtin.c \
	../execution/execution_simple_command.c \
	../execution/execution.h \
	../builtin/builtin.h \
	../builtin/boolean.c \
	../builtin/echo.c

libeval_a_CPPFLAGS = -I$(top_srcdir)/src

libeval_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

noinst_LIBRARIES = libeval.a
