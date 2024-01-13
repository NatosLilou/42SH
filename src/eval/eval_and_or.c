#include "eval.h"

int eval_and_or(struct ast_and_or *ast)
{
    if (ast->pipeline == NULL)
    {
        return 0;
    }
    return eval_pipeline(ast->pipeline);
}
