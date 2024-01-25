#include "eval.h"

static int temp(struct ast_simple_command *ast)
{
    char **temp = calloc(ast->pos_cmd + 1, sizeof(char *));
    for (size_t i = 0; i < ast->pos_cmd; i++)
    {
        temp[i] = calloc(strlen(ast->commands[i]) + 1, sizeof(char));
        strcpy(temp[i], ast->commands[i]);
    }
    for (size_t i = 0; i < ast->pos_cmd; i++)
    {
        temp[i] = expand(temp[i]);
    }
    int res = execution_simple_command(temp);
    for (size_t i = 0; i < ast->pos_cmd; i++)
    {
        free(temp[i]);
    }
    free(temp);
    return res;
}

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
        return temp(ast);
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
