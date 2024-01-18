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
    /*
    if (ast->rule_while)
    {
        return eval_rule_while(ast->rule_while);
    }
    if (ast->rule until)
    {
        return eval_rule_until(ast->rule_until);
    }
    */
   return 0;
}
