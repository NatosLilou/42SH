#include "execution.h"

int execution_builtin(char **args, enum builtin builtin, int loop_stage)
{
    if (builtin == ECHO)
        return echo(args, stdout);
    if (builtin == BOOL_FALSE)
        return my_false();
    if (builtin == BOOL_TRUE)
        return my_true();
    if (builtin == CD)
        return cd(args);
    if (builtin == EXPORT)
        return my_export(args);
    if (builtin == EXIT)
        return my_exit(args);
    if (builtin == CONTINUE)
        return my_continue(args, loop_stage);
    if (builtin == BREAK)
        return my_break(args, loop_stage);
    return unset(args);
}
