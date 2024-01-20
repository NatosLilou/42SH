#include "execution.h"

int execution_vp(char **args)
{
    int pid = fork();
    if (pid == 0)
    {
        execvp(args[0], args);
        perror("execvp failed");
        _exit(127);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        fflush(stdout);
        return WEXITSTATUS(status);
    }
    return 0;
}
