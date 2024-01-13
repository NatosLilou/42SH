#include "eval.h"

int eval_simple_command(struct ast_simple_command *ast)
{
    return execution_simple_command(ast->commands);
}
