#include "eval.h"

extern struct assigned_var *assigned;


void reverse_redir_simple_command(struct ast_simple_command *ast)
{
    for (size_t i = ast->pos_restore; i > 0; i--)
    {
        fflush(stdout);
        dup2(ast->restore[i - 1][0], ast->restore[i -1][1]);
        close(ast->restore[i - 1][0]);
    }
    ast->pos_restore = 0;
}

static int temp(struct ast_simple_command *ast)
{
    char **temp = calloc(ast->pos_cmd + 1, sizeof(char *));
    for (size_t i = 0; i < ast->pos_cmd; i++)
    {
        temp[i] = calloc(strlen(ast->commands[i]) + 1, sizeof(char));
        strcpy(temp[i], ast->commands[i]);
    }
    temp[0] = expand(temp[0]);
    struct ast_shell_command *arbuste = expand_func(temp[0]);
    for (size_t i = 1; i < ast->pos_cmd; i++)
    {
        temp[i] = expand(temp[i]);
    }
    int res = 0;;
    if (arbuste)
    {
        assigned->fun_args = realloc(assigned->fun_args, ast->pos_cmd * sizeof(char *));
        assigned->pos_fun_args = 0;
        for (size_t i = 1; i < ast->pos_cmd; i++)
        {
            assigned->fun_args[assigned->pos_fun_args] = temp[i];
            assigned->pos_fun_args++;
        }
        assigned->in_func = true;
        res = eval_shell_command(arbuste);
        assigned->in_func = false;
    }
    else
    {
        res = execution_simple_command(temp);
        reverse_redir_simple_command(ast);
    }
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
            res_redir = eval_redir_simple(ast->prefix[i]->redir, ast);
            if (res_redir != 0)
            {
                return 1;
            }
        }
    }
    while (i < ast->pos_redir)
    {
        res_redir = eval_redir_simple(ast->redir[i], ast);
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
