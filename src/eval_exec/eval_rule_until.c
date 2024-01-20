#include "eval.h"

int eval_rule_until(struct ast_rule_until *ast)
{
    int ret = 0;
    while (eval_compound_list(ast->compound_list_until))
        ret = eval_compound_list(ast->compound_list_do);
    return ret;
}
