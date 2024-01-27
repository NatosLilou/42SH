#include "execution.h"

extern struct assigned_var *arbre;

static void add_redir_simple(int fd1, int fd2, struct ast_simple_command *ast)
{
    ast->pos_restore++;
    ast->restore = realloc(ast->restore, (ast->pos_restore) * sizeof(int *));
    ast->restore[ast->pos_restore - 1] = calloc(2, sizeof(int));
    ast->restore[ast->pos_restore - 1][1] = fd1;
    ast->restore[ast->pos_restore - 1][0] = fd2;
}

static void add_redir(int fd1, int fd2, struct ast_command *ast)
{
    ast->pos_restore++;
    ast->restore = realloc(ast->redir, (ast->pos_restore) * sizeof(int *));
    ast->restore[ast->pos_restore - 1] = calloc(2, sizeof(int));
    ast->restore[ast->pos_restore - 1][1] = fd1;
    ast->restore[ast->pos_restore - 1][0] = fd2;
}

static int redir_great_simple(struct ast_redir *ast,
                              struct ast_simple_command *arbre)
{
    int stdout_dup = dup(ast->ionumber);
    int fd_out = open(ast->dest, O_CREAT | O_TRUNC | O_WRONLY, 0755);
    if (fd_out == -1)
    {
        err(1, "%s: No such file or directory\n", ast->dest);
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
    add_redir_simple(ast->ionumber, stdout_dup, arbre);
    return 0;
}

static int redir_dgreat_simple(struct ast_redir *ast,
                               struct ast_simple_command *arbre)
{
    int stdout_dup = dup(ast->ionumber);
    int fd_out = open(ast->dest, O_CREAT | O_APPEND | O_WRONLY, 0755);
    if (fd_out == -1)
    {
        err(1, "%s: No such file or directory\n", ast->dest);
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
    add_redir_simple(ast->ionumber, stdout_dup, arbre);
    return 0;
}

static int redir_less_simple(struct ast_redir *ast,
                             struct ast_simple_command *arbre)
{
    int stdout_dup = dup(ast->ionumber);
    int fd_in = open(ast->dest, O_RDONLY, 0666);
    if (fd_in == -1)
    {
        err(1, "%s: No such file or directory\n", ast->dest);
        close(stdout_dup);
        return 1;
    }
    if (dup2(fd_in, ast->ionumber) == -1)
    {
        err(1, "dup2 failed");
        close(stdout_dup);
        close(fd_in);
        return 1;
    }
    add_redir_simple(ast->ionumber, stdout_dup, arbre);
    return 0;
}

int redir_lessgreat_simple(struct ast_redir *ast,
                           struct ast_simple_command *arbre)
{
    int stdout_dup = dup(ast->ionumber);
    int fd_out = open(ast->dest, O_CREAT | O_RDWR, 0666);
    if (fd_out == -1)
    {
        err(1, "%s: No such file or directory\n", ast->dest);
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
    add_redir_simple(ast->ionumber, stdout_dup, arbre);
    return 0;
}

static int redir_lessand_simple(struct ast_redir *ast,
                                struct ast_simple_command *arbre)
{
    int fd = atoi(ast->dest);
    int stdin_dup = dup(ast->ionumber);
    if (dup2(fd, ast->ionumber) == -1)
    {
        err(1, "dup2 failed");
        close(stdin_dup);
        return 1;
    }
    add_redir_simple(ast->ionumber, stdin_dup, arbre);
    return 0;
}

static int redir_greatand_simple(struct ast_redir *ast,
                                 struct ast_simple_command *arbre)
{
    int fd = atoi(ast->dest);
    int stdout_dup = dup(ast->ionumber);
    if (dup2(fd, ast->ionumber) == -1)
    {
        err(1, "dup2 failed");
        close(stdout_dup);
        return 1;
    }
    add_redir_simple(ast->ionumber, stdout_dup, arbre);
    return 0;
}

//===============================

static int redir_great(struct ast_redir *ast, struct ast_command *arbre)
{
    int stdout_dup = dup(ast->ionumber);
    int fd_out = open(ast->dest, O_CREAT | O_TRUNC | O_WRONLY, 0755);
    if (fd_out == -1)
    {
        err(1, "%s: No such file or directory", ast->dest);
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
    add_redir(ast->ionumber, stdout_dup, arbre);
    return 0;
}

static int redir_dgreat(struct ast_redir *ast, struct ast_command *arbre)
{
    int stdout_dup = dup(ast->ionumber);
    int fd_out = open(ast->dest, O_CREAT | O_APPEND | O_WRONLY, 0755);
    if (fd_out == -1)
    {
        err(1, "%s: No such file or directory", ast->dest);
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
    add_redir(ast->ionumber, stdout_dup, arbre);
    return 0;
}

static int redir_less(struct ast_redir *ast, struct ast_command *arbre)
{
    int stdout_dup = dup(ast->ionumber);
    int fd_in = open(ast->dest, O_RDONLY, 0666);
    if (fd_in == -1)
    {
        err(1, "%s: No such file or directory", ast->dest);
        close(stdout_dup);
        return 1;
    }
    if (dup2(fd_in, ast->ionumber) == -1)
    {
        err(1, "dup2 failed");
        close(stdout_dup);
        close(fd_in);
        return 1;
    }
    add_redir(ast->ionumber, stdout_dup, arbre);
    return 0;
}

int redir_lessgreat(struct ast_redir *ast, struct ast_command *arbre)
{
    int stdout_dup = dup(ast->ionumber);
    int fd_out = open(ast->dest, O_CREAT | O_RDWR, 0666);
    if (fd_out == -1)
    {
        err(1, "%s: No such file or directory", ast->dest);
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
    add_redir(ast->ionumber, stdout_dup, arbre);
    return 0;
}

static int redir_lessand(struct ast_redir *ast, struct ast_command *arbre)
{
    int fd = atoi(ast->dest);
    int stdin_dup = dup(ast->ionumber);
    if (dup2(fd, ast->ionumber) == -1)
    {
        err(1, "dup2 failed");
        close(stdin_dup);
        return 1;
    }
    add_redir(ast->ionumber, stdin_dup, arbre);
    return 0;
}

static int redir_greatand(struct ast_redir *ast, struct ast_command *arbre)
{
    int fd = atoi(ast->dest);
    int stdout_dup = dup(ast->ionumber);
    if (dup2(fd, ast->ionumber) == -1)
    {
        err(1, "dup2 failed");
        close(stdout_dup);
        return 1;
    }
    add_redir(ast->ionumber, stdout_dup, arbre);
    return 0;
}

int execution_redir_simple(struct ast_redir *ast,
                           struct ast_simple_command *arbre)
{
    if (!ast)
    {
        return 1;
    }
    switch (ast->redir)
    {
    case TOKEN_LESS: // <
        return redir_less_simple(ast, arbre);
    case TOKEN_GREAT: // >
        return redir_great_simple(ast, arbre);
    case TOKEN_DGREAT: // >>
        return redir_dgreat_simple(ast, arbre);
    case TOKEN_GREATAND: // >&
        return redir_greatand_simple(ast, arbre);
    case TOKEN_LESSAND: // <&
        return redir_lessand_simple(ast, arbre);
    case TOKEN_CLOBBER: // >|
        return redir_great_simple(ast, arbre);
    case TOKEN_LESSGREAT: // <>
        return redir_lessgreat_simple(ast, arbre);
    default:
        break;
    }
    return 1;
}

int execution_redir(struct ast_redir *ast, struct ast_command *arbre)
{
    if (!ast)
    {
        return 1;
    }
    switch (ast->redir)
    {
    case TOKEN_LESS: // <
        return redir_less(ast, arbre);
    case TOKEN_GREAT: // >
        return redir_great(ast, arbre);
    case TOKEN_DGREAT: // >>
        return redir_dgreat(ast, arbre);
    case TOKEN_GREATAND: // >&
        return redir_greatand(ast, arbre);
    case TOKEN_LESSAND: // <&
        return redir_lessand(ast, arbre);
    case TOKEN_CLOBBER: // >|
        return redir_great(ast, arbre);
    case TOKEN_LESSGREAT: // <>
        return redir_lessgreat(ast, arbre);
    default:
        break;
    }
    return 1;
}
