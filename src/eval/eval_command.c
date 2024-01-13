#include "eval.h"

int eval_command(struct ast_command *ast)
{
    if (ast->simple_command)
    {
        return eval_simple_command(ast->simple_command);
    }
    if (ast->shell_command)
    {
        return eval_shell_command(ast->shell_command);
    }

    return 0;
}
