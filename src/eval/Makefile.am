# Name of the libaries
lib_LIBRARIES = libeval.a

# List of file to compile in libeval.a
libeval_a_SOURCES =  \
	eval.c eval.h \
	../ast/ast_and_or.c \
	../ast/ast_command.c \
	../ast/ast_input.c \
	../ast/ast_list.c \
	../ast/ast_pipeline.c \
	../ast/ast_simple_command.c \
	../ast/ast.h \
	../execution/execution.c ../execution/execution.h \
	../builtin/builtin.h \
	../builtin/boolean.c \
	../builtin/echo.c

libeval_a_CPPFLAGS = -I$(top_srcdir)/src

libeval_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

noinst_LIBRARIES = libeval.a
