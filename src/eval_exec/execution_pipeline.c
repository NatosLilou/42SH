#include "execution.h"

int execution_pipeline(struct ast_pipeline *ast)
{
    int fds[2] = {STDIN_FILENO, STDOUT_FILENO};
    int fds_temp[2];
    int res;
    size_t i = 0;
    while (i <= ast->pos)
    {
        pipe(fds_temp);
        pid_t pid = fork();
        if (pid != 0)
        {
            if (i != 0)
            {
                close(fds[0]);
                close(fds[1]);
            }
            i++;
            fds[0] = fds_temp[0];
            fds[1] = fds_temp[1];
            int wstatus;
            waitpid(pid,&wstatus,0);
            res = WEXITSTATUS(wstatus);
        }
        else
        {
            if (i != 2)
                dup2(fds_temp[1], STDOUT_FILENO);
            if (i != 0)
            {
                dup2(fds[0], STDIN_FILENO);
                close(fds[1]);
            }
            close(fds[0]);
            close(fds_temp[0]);
            close(fds_temp[1]);
            int res = eval_command(ast->commands[i]);
            if (res == 0)
            {
                exit(EXIT_SUCCESS);
            }
            errx(res, "execvp failed");
        }
    }
    close(fds[1]);
    close(fds[0]);
    return res;
}
