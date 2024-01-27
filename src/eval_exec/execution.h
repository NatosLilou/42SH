#ifndef EXECUTION_H
#define EXECUTION_H

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "builtin/builtin.h"
#include "eval.h"

enum builtin
{
    ECHO,
    BOOL_TRUE,
    BOOL_FALSE,
    EXPORT,
    UNSET,
    EXIT,
    UNKNOWN
};

int execution_simple_command(char **args);
int execution_builtin(char **args, enum builtin builtin);
int execution_vp(char **args);
int execution_pipeline(struct ast_pipeline *ast);
int execution_redir(struct ast_redir *ast, struct ast_command *arbre);
int execution_redir_simple(struct ast_redir *ast,
                           struct ast_simple_command *arbre);
#endif /* !EXECUTION_H */
