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
    if (ast->rule_while)
    {
        return eval_rule_while(ast->rule_while);
    }
    if (ast->rule_until)
    {
        return eval_rule_until(ast->rule_until);
    }
    if (ast->rule_for)
    {
        return eval_rule_for(ast->rule_for);
    }
    if (ast->compound_list)
    {
        if (ast->sub)
        {
            return eval_subshell(ast->compound_list);
        }
        else
        {
            return eval_command_blocks(ast->compound_list);
        }
    }
    if (ast->rule_case)
    {
        return eval_rule_case(ast->rule_case);
    }
    return 0;
}
