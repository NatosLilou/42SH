#include "eval.h"

int eval_pipeline(struct ast_pipeline *ast)
{
    if (ast->commands == NULL)
    {
        return 0;
    }

    return eval_command(*ast->commands);
}
