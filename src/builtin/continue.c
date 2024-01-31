#include <err.h>

#include "builtin.h"

extern struct assigned_var *assigned;

int my_continue(char **argv, int loop_stage)
{
    if (argv[1])
    {
        if (argv[2])
        {
            err(1, "continue: too many arguments");
            assigned->exiting = 1;
        }
        else
        {
            if (strcmp(argv[0], "0") == 0)
            {
                err(1, "continue:  out of loop");
                assigned->exiting = 1;
                return -42;
            }
            int n = atoi(argv[1]);
            if (n == 0)
            {
                err(128, "exit: not a numeric number");
                assigned->exiting = 128;
            }
            if (n < 0)
            {
                err(1, "continue:  out of loop");
                assigned->exiting = 1;
            }
            else
            {
                return loop_stage + n - 1;
            }
        }
    }
    else
    {
        return loop_stage;
    }

    return -42;
}
