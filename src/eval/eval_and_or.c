#include "eval.h"

int eval_and_or(struct ast_and_or *ast)
{
    return eval_pipeline(ast->pipeline);
}