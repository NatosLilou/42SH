#include "execution.h"
int redir_great(struct ast_redir *ast)
{
    int stdout_dup = dup(ast->ionumber);
    int fd_out = open(ast->dest, O_CREAT | O_TRUNC | O_WRONLY, 0666);
    if (fd_out == -1)
    {
        err(1, "open failed");
        close(stdout_dup);
        return 1;
    }

    if (dup2(fd_out, ast->ionumber) == -1)
    {
        err(1, "dup2 failed");
        close(stdout_dup);
        close(fd_out);
        return 1;
    }
    fd_out = fcntl(fd_out, F_SETFD, FD_CLOEXEC);
    stdout_dup = fcntl(stdout_dup, F_SETFD, FD_CLOEXEC);
    close(stdout_dup);
    return 0;
}

static int redir_dgreat(struct ast_redir *ast)
{
    int stdout_dup = dup(ast->ionumber);
    int fd_out = open(ast->dest, O_CREAT | O_APPEND | O_WRONLY, 0666);

    if (fd_out == -1)
    {
        err(1, "open failed");
        close(stdout_dup);
        return 1;
    }

    if (dup2(fd_out, ast->ionumber) == -1)
    {
        err(1, "dup2 failed");
        close(stdout_dup);
        close(fd_out);
        return 1;
    }
    fd_out = fcntl(fd_out, F_SETFD, FD_CLOEXEC);
    stdout_dup = fcntl(stdout_dup, F_SETFD, FD_CLOEXEC);
    close(stdout_dup);
    return 0;
}

static int redir_less(struct ast_redir *ast)
{
    int stdin_dup = dup(ast->ionumber);
    int fd_in = open(ast->dest, O_RDONLY, 0666);
    if (fd_in == -1)
    {
        err(1, "open failed");
        close(stdin_dup);
        return 1;
    }

    if (dup2(fd_in, ast->ionumber) == -1)
    {
        err(1, "dup2 failed");
        close(stdin_dup);
        close(fd_in);
        return 1;
    }
    fd_in = fcntl(fd_in, F_SETFD, FD_CLOEXEC);
    stdin_dup = fcntl(stdin_dup, F_SETFD, FD_CLOEXEC);
    close(stdin_dup);
    return 0;
}

int redir_lessgreat(struct ast_redir *ast)
{
    int stdout_dup = dup(ast->ionumber);
    int fd_out = open(ast->dest, O_CREAT | O_RDWR, 0666);
    if (fd_out == -1)
    {
        err(1, "open failed");
        close(stdout_dup);
        return 1;
    }

    if (dup2(fd_out, ast->ionumber) == -1)
    {
        err(1, "dup2 failed");
        close(stdout_dup);
        close(fd_out);
        return 1;
    }
    fd_out = fcntl(fd_out, F_SETFD, FD_CLOEXEC);
    stdout_dup = fcntl(stdout_dup, F_SETFD, FD_CLOEXEC);

    close(stdout_dup);
    return 0;
}

static int redir_lessand(struct ast_redir *ast)
{
    int fd = atoi(ast->dest);
    int stdin_dup = dup(ast->ionumber);

    if (dup2(fd, ast->ionumber) == -1)
    {
        err(1, "dup2 failed");
        close(fd);
        close(stdin_dup);
        return 1;
    }
    fd = fcntl(fd, F_SETFD, FD_CLOEXEC);
    stdin_dup = fcntl(stdin_dup, F_SETFD, FD_CLOEXEC);
    close(stdin_dup);
    return 0;
}

static int redir_greatand(struct ast_redir *ast)
{
    int fd = atoi(ast->dest);
    int stdout_dup = dup(ast->ionumber);

    if (dup2(fd, ast->ionumber) == -1)
    {
        err(1, "dup2 failed");
        close(fd);
        close(stdout_dup);
        return 1;
    }
    fd = fcntl(fd, F_SETFD, FD_CLOEXEC);
    stdout_dup = fcntl(stdout_dup, F_SETFD, FD_CLOEXEC);
    close(stdout_dup);
    return 0;
}

int execution_redir(struct ast_redir *ast)
{
    if (!ast)
    {
        return 1;
    }
    switch (ast->redir)
    {
    case TOKEN_LESS: // <
        return redir_less(ast);
    case TOKEN_GREAT: // >
        return redir_great(ast);
    case TOKEN_DGREAT: // >>
        return redir_dgreat(ast);
    case TOKEN_GREATAND: // >&
        return redir_greatand(ast);
    case TOKEN_LESSAND: // <&
        return redir_lessand(ast);
    case TOKEN_CLOBBER: // >|
        return redir_great(ast);
    case TOKEN_LESSGREAT: // <>
        return redir_lessgreat(ast);
    default:
        break;
    }
    return 1;
}
