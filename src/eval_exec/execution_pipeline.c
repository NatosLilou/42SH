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
        if (*res == 0)
        {
            exit(EXIT_SUCCESS);
        }
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
        pid_t pid_exec = execute(ast->commands[i], input_fd, (i + 1 < ast->pos) ? fds[1] : STDOUT_FILENO, &res);
        waitpid(pid_exec, &wstatus, 0);

        if (input_fd != STDIN_FILENO)
        {
            close(input_fd);
        }

        // Prepare l'entrée pour la prochaine commande
        if (i + 1 < ast->pos)
        {
            close(fds[1]);
            input_fd = fds[0];
        }
        i++;
    }

    return WEXITSTATUS(wstatus);
}
