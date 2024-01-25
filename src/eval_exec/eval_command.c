#include "eval.h"

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
            if (eval_redir(ast->redir[i]) == 1)
            {
                return 1;
            }
            i++;
        }
        return eval_shell_command(ast->shell_command);
    }
    if (ast->funcdec)
    {
        size_t i = 0;
        while (i < ast->pos)
        {
            if (eval_redir(ast->redir[i]) == 1)
            {
                return 1;
            }
            i++;
        }
        return eval_funcdec(ast->funcdec);
    }

    return 0;
}
