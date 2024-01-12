#include <stdio.h>

#include "../builtin/builtin.h"

int execution(char **args)
{
    return echo(args, stdout);
}
