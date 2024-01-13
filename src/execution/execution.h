#ifndef EXECUTION_H
#define EXECUTION_H

#include <stdio.h>
#include <string.h>

#include "../builtin/builtin.h"

enum builtin
{
    ECHO,
    BOOL_TRUE,
    BOOL_FALSE,
    UNKNOWN
};

int execution_simple_command(char **args);
int execution_builtin(char **args, enum builtin builtin);

#endif /* !EXECUTION_H */
