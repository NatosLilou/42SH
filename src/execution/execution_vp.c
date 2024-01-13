#include "execution.h"

int execution_vp(char **args)
{
    int pid = fork();
    if (pid == 0)
    {
        execvp(args[0], args);
        err(127, "execvp failed");
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        fflush(stdout);
        if (WEXITSTATUS(status) == 127)
        {
            return 1;
        }
    }
    return 0;
}
