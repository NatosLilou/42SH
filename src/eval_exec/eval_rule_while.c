#include "eval.h"

int eval_rule_while(struct ast_rule_while *ast)
{
    int ret = 0;
    while (!eval_compound_list(ast->compound_list_while)
           && (ret <= ast->loop_stage || ret >= 0))
        ret = eval_compound_list(ast->compound_list_do);
    return ret;
}
