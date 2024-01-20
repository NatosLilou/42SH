#include <stdlib.h>

#include "ast.h"

struct ast_rule_while *new_ast_rule_while(void)
{
    struct ast_rule_while *new = calloc(1, sizeof(struct ast_rule_while));

    new->type = AST_RULE_WHILE;
    new->compound_list_while = NULL;
    new->compound_list_do = NULL;

    return new;
}

void free_ast_rule_while(struct ast_rule_while *ast)
{
    if (ast)
    {
        if (ast->compound_list_while)
        {
            free_ast_compound_list(ast->compound_list_while);
        }

        if (ast->compound_list_do)
        {
            free_ast_compound_list(ast->compound_list_do);
        }

        free(ast);
    }
}
