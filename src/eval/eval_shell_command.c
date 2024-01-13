#include "eval.h"

int eval_shell_command(struct ast_shell_command *ast)
{
    if (!ast->rule_if)
    {
        return 0;
    }
    return eval_rule_if(ast->rule_if);
}
