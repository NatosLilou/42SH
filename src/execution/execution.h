#ifndef EXECUTION_H
#define EXECUTION_H

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

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
int execution_vp(char **args);

#endif /* !EXECUTION_H */
