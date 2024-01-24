#include "eval.h"

int eval_simple_command(struct ast_simple_command *ast)
{
    int res_redir = 0;
    size_t i = 0;
    if (ast->prefix)
    {
        for (size_t i = 0; i < ast->pos_pref && ast->prefix[i]->redir; i++)
        {
            res_redir = eval_redir(ast->prefix[i]->redir);
            if (res_redir != 0)
            {
                return 1;
            }
        }
    }
    while (i < ast->pos_redir)
    {
        res_redir = eval_redir(ast->redir[i]);
        if (res_redir != 0)
        {
            return 1;
        }
        i++;
    }
    if (ast->commands && ast->commands[0])
    {
        for (size_t i = 0; i < ast->pos_cmd; i++)
        {
            ast->commands[i] = expand(ast->commands[i]);
        }
        return execution_simple_command(ast->commands);
    }
    if (ast->prefix)
    {
        for (size_t i = 0; i < ast->pos_pref; i++)
        {
            eval_prefix(ast->prefix[i]);
        }

        return 0;
    }

    return 1;
}
