#include "execution.h"


static pid_t exec_fork(struct ast_command *ast, int fds[2], int side)
{
    pid_t pid = fork();
    if (pid != 0)
        return pid;

    int fd_to = side == 0 ? STDOUT_FILENO : STDIN_FILENO;
    int fd_final = side == 0 ? fds[1] : fds[0];

    if (dup2(fd_final, fd_to) == -1)
    {
        errx(1, "dup2 failed");
    }
    int res = eval_command(ast);
    close(fds[0]);
    close(fds[1]);
    if (res == 1)
        errx(1, "eval_failed %d failed", side);
   exit(EXIT_SUCCESS);
}

int execution_pipeline(struct ast_pipeline *ast)
{
    int fds[2];
    if (pipe(fds) == -1)
    {
        errx(1, "pipe failed");
    }
    int wstatus;
    int pid_first = exec_fork(ast->commands[0], fds, 0);
    waitpid(pid_first, &wstatus, 0);
    int res = WEXITSTATUS(wstatus);
    printf("the first exit = %d \n", res);
    printf("ast->pos = %zu\n", ast->pos);
    size_t i = 1;
    while (i < ast->pos)
    {
        printf("hey i try the %zu enieme command\n", i);
        int pid_right = exec_fork(ast->commands[i], fds, 1);
        waitpid(pid_right, &wstatus, 0);
        res = WEXITSTATUS(wstatus);
        i++;
    }

    close(fds[0]);
    close(fds[1]);
    return res;
}
