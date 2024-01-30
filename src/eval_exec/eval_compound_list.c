#include "eval.h"

int eval_compound_list(struct ast_compound_list *ast)
{
    if (!ast->and_or)
    {
        return 0;
    }

    int res = 0;
    for (size_t i = 0; i < ast->pos && res > -42; i++)
    {
        res = eval_and_or(ast->and_or[i]);
    }

    return res;
}
