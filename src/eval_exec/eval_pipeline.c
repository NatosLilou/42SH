#include "eval.h"

int eval_pipeline(struct ast_pipeline *ast)
{
    if (!ast)
        return 0;
    if (ast->negation)
    {
        return !execution_pipeline(ast);
    }
    return execution_pipeline(ast);
}
