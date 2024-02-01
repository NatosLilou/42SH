#include "execution.h"

static enum builtin check_builtin(char *command_name)
{
    if (strcmp(command_name, "echo") == 0)
    {
        return ECHO;
    }
    if (strcmp(command_name, "true") == 0)
    {
        return BOOL_TRUE;
    }
    if (strcmp(command_name, "false") == 0)
    {
        return BOOL_FALSE;
    }
    if (strcmp(command_name, "cd") == 0)
    {
        return CD;
    }
    if (strcmp(command_name, "export") == 0)
    {
        return EXPORT;
    }
    if (strcmp(command_name, "unset") == 0)
    {
        return UNSET;
    }
    if (strcmp(command_name, "exit") == 0)
    {
        return EXIT;
    }
    if (strcmp(command_name, "break") == 0)
    {
        return BREAK;
    }
    if (strcmp(command_name, "continue") == 0)
    {
        return CONTINUE;
    }

    return UNKNOWN;
}

int execution_simple_command(char **args, int loop_stage)
{
    while (*args && !strcmp(*args, ""))
        ++args;
    if (!*args)
        return 0;
    enum builtin builtin = check_builtin(args[0]);
    if (builtin == UNKNOWN)
    {
        return execution_vp(args);
    }
    int res = execution_builtin(args, builtin, loop_stage);
    fflush(stdout);
    return res;
}
