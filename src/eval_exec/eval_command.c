#include "eval.h"

void reverse_redir_command(struct ast_command *ast)
{
    for (size_t i = ast->pos_restore; i > 0; i--)
    {
        fflush(stdout);
        dup2(ast->restore[i - 1][0], ast->restore[i - 1][1]);
        close(ast->restore[i - 1][0]);
    }
}

int eval_command(struct ast_command *ast)
{
    if (ast->simple_command)
    {
        return eval_simple_command(ast->simple_command);
    }
    if (ast->shell_command)
    {
        size_t i = 0;
        while (i < ast->pos)
        {
            if (eval_redir(ast->redir[i], ast) == 1)
            {
                return 1;
            }
            i++;
        }
        int res = eval_shell_command(ast->shell_command);
        reverse_redir_command(ast);
        return res;
    }
    if (ast->funcdec)
    {
        size_t i = 0;
        while (i < ast->pos)
        {
            if (eval_redir(ast->redir[i], ast) == 1)
            {
                return 1;
            }
            i++;
        }
        int res = eval_funcdec(ast->funcdec);
        reverse_redir_command(ast);
        return res;
    }

    return 0;
}
