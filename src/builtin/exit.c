#include <err.h>

#include "builtin.h"

extern struct assigned_var *assigned;

int my_exit(char **argv)
{
    if (argv[1])
    {
        if (strcmp(argv[1], "0") == 0)
        {
            assigned->exiting = 0;
            return -42;
        }
        int n = atoi(argv[1]);
        if (n == 0)
        {
            err(2, "exit: not a numeric number");
            assigned->exiting = 2;
        }
        else
        {
            assigned->exiting = n;
        }
    }
    else
    {
        assigned->exiting = assigned->exit_code;
    }

    return -42;
}
