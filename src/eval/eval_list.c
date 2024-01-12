#include "eval.h"

int eval_list(struct ast_list *ast)
{
    if (ast->and_or == NULL)
    {
        return 0;
    }
    int res = 1;
    for (size_t i = 0; ast->and_or[i]; i++)
    {
        res = eval_and_or(ast->and_or[i]);
    }
    return res;
}