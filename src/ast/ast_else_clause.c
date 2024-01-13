#include <stdlib.h>

#include "ast.h"

struct ast_else_clause *new_ast_else_clause(void)
{
    struct ast_else_clause *new = calloc(1, sizeof(struct ast_else_clause));

    new->type = AST_ELSE_CLAUSE;
    new->compound_list_elif = NULL;
    new->compound_list_then = NULL;
    new->else_clause = NULL;

    return new;
}

void print_ast_else_clause(struct ast_else_clause *ast)
{
    if (!ast)
    {
        return;
    }

    printf("AST_ELSE_CLAUSE\n");

    print_ast_compound_list(ast->compound_list_elif);
    print_ast_compound_list(ast->compound_list_then);
    print_ast_else_clause(ast->else_clause);
}

void free_ast_else_clause(struct ast_else_clause *ast)
{
    if (ast->compound_list_elif)
    {
        free_ast_compound_list(ast->compound_list_elif);
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
