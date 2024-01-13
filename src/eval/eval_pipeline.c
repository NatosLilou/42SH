#include "eval.h"

int eval_pipeline(struct ast_pipeline *ast)
{
    if (ast->command == NULL)
    {
        return 0;
    }

    return eval_command(ast->command);
}
