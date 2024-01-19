#include "execution.h"

pid_t execute(struct ast_command *ast, int input_fd, int ouput_fd, int *res)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        if (input_fd != STDIN_FILENO)
        {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }

        if (ouput_fd != STDOUT_FILENO)
        {
            dup2(ouput_fd, STDOUT_FILENO);
            close(ouput_fd);
        }


        *res = eval_command(ast);
        //printf("In fork exec res = %d\n", *res);
        if (*res == 0)
        {
            //printf("In fork exec res = %d\n", *res);
            exit(EXIT_SUCCESS);
        }
        //execvp(ast->simple_command->commands[0], ast->simple_command->commands);

        //perror("execvp");
        exit(EXIT_FAILURE);
    }
    else
        return pid;
}


int execution_pipeline(struct ast_pipeline *ast)
{
    int input_fd = STDIN_FILENO;
    int fds[2];
    if (pipe(fds) == -1)
    {
        errx(1, "pipe failed");
    }
    int res = 0;
    size_t i = 0;
    int wstatus;
    while (i < ast->pos)
    {
        if (i + 1 < ast->pos && pipe(fds) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        //printf("i = %zu Before exec res = %d\n", i, res);
        pid_t pid_exec = execute(ast->commands[i], input_fd, (i + 1 < ast->pos) ? fds[1] : STDOUT_FILENO, &res);
        waitpid(pid_exec, &wstatus, 0);
        //printf("i = %zu After exec res = %d\n", i, res);
        if (input_fd != STDIN_FILENO)
        {
            close(input_fd);
        }

        if (i + 1 < ast->pos)
        {
            close(fds[1]);
            input_fd = fds[0];
        }
        i++;
    }
    //while (wait(&wstatus) > 0);

    return WEXITSTATUS(wstatus);
}
