#include "eval.h"

int eval_pipeline(struct ast_pipeline *ast)
{
    eval_command(ast->command);
}