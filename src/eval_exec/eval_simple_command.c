#include "eval.h"

int eval_simple_command(struct ast_simple_command *ast)
{
    int res_redir = 0;
    size_t i = 0;
    while (i < ast->pos_redir)
    {
        res_redir = eval_redir(ast->redir[i]);
        if (res_redir != 0)
        {
            return 1;
        }
        i++;
    }
    if (ast->commands)
    {
        return execution_simple_command(ast->commands); // C'est nul
    }

    return 1;
}
