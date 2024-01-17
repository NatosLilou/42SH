#include <stdlib.h>

#include "ast.h"

struct ast_rule_until *new_ast_rule_until(void)
{
    struct ast_rule_until *new = calloc(1, sizeof(struct ast_rule_until));

    new->type = AST_RULE_WHILE;
    new->compound_list_until = NULL;
    new->compound_list_do = NULL;

    return new;
}

void print_ast_rule_until(struct ast_rule_until *ast)
{
    if (!ast)
    {
        return;
    }

    printf("AST_RULE_UNTIL\n");
}

void free_ast_rule_until(struct ast_rule_until *ast)
{
    if (ast->compound_list_until)
    {
        free_ast_compound_list(ast->compound_list_until);
    }

    if (ast->compound_list_do)
    {
        free_ast_compound_list(ast->compound_list_do);
    }

    free(ast);
}
