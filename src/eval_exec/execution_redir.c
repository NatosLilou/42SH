#include "execution.h"
int redir_great(struct ast_redir *ast)
{

    int stdout_dup = dup(ast->ionumber);
    int fd_out = open(ast->dest, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if (fd_out == -1)
    {
        err(1, "open failed");
    }

    if (dup2(fd_out, ast->ionumber) == -1)
        err(1, "dup2 failed");
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
        //case TOKEN_LESS: // <
        //    return redir_less(ast);
        case TOKEN_GREAT: // >
            return redir_great(ast);
        /*case TOKEN_DGREAT: // >>
            return redir_dgreat(ast);
        case TOKEN_GREATAND: // >&
            return redir_greatand(ast);
        case TOKEN_lessand: // <&
            return redir_lessand(ast);
        case TOKEN_CLOBBER: // >|
            return redir_clobber(ast);
        case TOKEN_LESSGREAT: // <>
            return redir_lessgreat(ast);*/
        default:
                break;
    }
    return 1;
}
