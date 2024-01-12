#include "execution.h"

static enum builtin check_builtin(char *command_name)
{
    if(strcmp(command_name, "echo") == 0)
    {
        return ECHO;
    }
    if(strcmp(command_name, "true") == 0)
    {
        return BOOL_TRUE;
    }
    if(strcmp(command_name, "false") == 0)
    {
        return BOOL_FALSE;
    }
    return UNKNOWN;
}

int execution_simple_command(char **args)
{
    enum builtin builtin = check_builtin(args[0]);
    if (builtin == UNKNOWN)
    {
        //TODO check if we have something else than builtin later
        return 1;
    }
    return execution_builtin(args, builtin);
}