#include "execution.h"

int execution_builtin(char **args, enum builtin builtin)
{
    if (builtin == ECHO)
        return echo(args, stdout);
    if (builtin == BOOL_FALSE)
        return my_false();
    return my_true();
}  