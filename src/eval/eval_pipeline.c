#include "eval.h"

int eval_pipeline(struct ast_pipeline *ast)
{
    if (!ast)
        return 0;   
    return execution_pipeline(ast);
}
