#include "eval.h"

int eval_shell_command(struct ast_shell_command *ast)
{
    if (!ast)
    {
        return 0;
    }
    if (ast->rule_if)
    {
        return eval_rule_if(ast->rule_if);
    }
    return 0;
}
