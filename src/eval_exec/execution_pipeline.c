#include "execution.h"

void execute(struct ast_command *ast, int input_fd, int ouput_fd)
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

        execvp(ast->simple_command->commands[0], ast->simple_command->commands);

        perror("execvp");
        exit(EXIT_FAILURE);
    }
}


int execution_pipeline(struct ast_pipeline *ast)
{
    int input_fd = STDIN_FILENO;
    int fds[2];
    if (pipe(fds) == -1)
    {
        errx(1, "pipe failed");
    }
    size_t i = 0;
    while (i < ast->pos)
    {
        if (i + 1 < ast->pos && pipe(fds) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        execute(ast->commands[i], input_fd, (i + 1 < ast->pos) ? fds[1] : STDOUT_FILENO);

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
    while (wait(NULL) > 0);

    return 0;
}
