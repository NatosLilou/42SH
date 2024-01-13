#include "eval.h"

int eval_rule_if(struct ast_rule_if *ast)
{
    if (!ast->compound_list_if || !ast->compound_list_then)
    {
        return 0;
    }

    int condition = eval_compound_list(ast->compound_list_if);
    if (condition == 0) // TRUE
    {
        return eval_compound_list(ast->compound_list_then);
    }
    else if (ast->else_clause)
    {
        return eval_else_clause(ast->else_clause);
    }

    return 0;
}
