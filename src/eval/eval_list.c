#include "eval.h"

int eval_list(struct ast_list *ast)
{
    int res = 1;
    for (size_t i = 0; i < ast->size; i++)
    {
        res = eval_and_or(ast->and_or[i]);
    }
    return res;
}