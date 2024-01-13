#include <stdlib.h>

#include "ast.h"

struct ast_rule_if *new_ast_rule_if(void)
{
    struct ast_rule_if *new = calloc(1, sizeof(struct ast_rule_if));

    new->type = AST_RULE_IF;
    new->compound_list_if = NULL;
    new->compound_list_then = NULL;
    new->else_clause = NULL;

    return new;
}

void print_ast_rule_if(struct ast_rule_if *ast)
{
    if (!ast)
    {
        return;
    }

    printf("AST_RULE_IF\n");

    print_ast_compound_list(ast->compound_list_if);
    print_ast_compound_list(ast->compound_list_then);
    print_ast_else_clause(ast->else_clause);
}

void free_ast_rule_if(struct ast_rule_if *ast)
{
    if (ast->compound_list_if)
    {
        free_ast_compound_list(ast->compound_list_if);
    }

    if (ast->compound_list_then)
    {
        free_ast_compound_list(ast->compound_list_then);
    }

    if (ast->else_clause)
    {
        free_ast_else_clause(ast->else_clause);
    }

    free(ast);
}
