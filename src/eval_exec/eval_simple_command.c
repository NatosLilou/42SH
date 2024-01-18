#include "eval.h"

int eval_simple_command(struct ast_simple_command *ast)
{
    if (ast->commands)
    {
        return execution_simple_command(ast->commands); // C'est nul
    }

    return 1;
}
