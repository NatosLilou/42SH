#include "execution.h"

int execution_builtin(char **args, enum builtin builtin)
{
    if (builtin == ECHO)
        return echo(args, stdout);
    if (builtin == BOOL_FALSE)
    {
        //printf("Hey I exec false\n");
        return my_false();
    }
    //printf("Hey I exec true\n");
    return my_true();
}  
