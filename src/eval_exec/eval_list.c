#include "eval.h"

extern struct assigned_var *assigned;

int eval_list(struct ast_list *ast)
{
    if (ast->and_or == NULL)
    {
        return 0;
    }
    int res = 1;
    for (size_t i = 0; i < ast->pos; i++)
    {
        res = eval_and_or(ast->and_or[i]);
        if (res == -42) // EXIT
        {
            return res;
        }
        assigned->exit_code = res;
    }
    return res;
}
