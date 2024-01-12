#include "eval.h"

int eval_command(struct ast_command *ast)
{
    if (ast->simple_command == NULL)
    {
        return 0;
    }
    //TODO recognize simple_command / shell_command
    return eval_simple_command(ast->simple_command);
}