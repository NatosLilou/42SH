#include <stdlib.h>

#include "ast.h"

struct ast_rule_case *new_ast_rule_case(void)
{
    struct ast_rule_case *new = calloc(1, sizeof(struct ast_rule_case));

    new->type = AST_RULE_CASE;

    new->word = NULL;
    new->case_clause = NULL;

    return new;
}

void free_ast_rule_case(struct ast_rule_case *ast)
{
    if (ast)
    {
        if (ast->word)
        {
            free(ast->word);
        }
        if (ast->case_clause)
        {
            free_ast_case_clause(ast->case_clause);
        }
        free(ast);
    }
}
