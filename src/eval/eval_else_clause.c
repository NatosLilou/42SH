#include "eval.h"

int eval_else_clause(struct ast_else_clause *ast)
{
    if (!ast->compound_list_elif)
    {
        return 0;
    }

    if (!ast->compound_list_then)
    {
        return eval_compound_list(ast->compound_list_elif);
    }

    int condition = eval_compound_list(ast->compound_list_elif);
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
