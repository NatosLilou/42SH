#include <stdlib.h>

#include "ast.h"

struct ast_compound_list *new_ast_compound_list(void)
{
    struct ast_compound_list *new = calloc(1, sizeof(struct ast_compound_list));

    new->type = AST_COMPOUND_LIST;
    new->and_or = NULL;

    return new;
}

void print_ast_compound_list(struct ast_compound_list *ast)
{
    if (!ast)
    {
        return;
    }

    printf("AST_COMPOUND_LIST\n");

    print_ast_and_or(ast->and_or);
}

void free_ast_compound_list(struct ast_compound_list *ast)
{
    if (ast->and_or)
    {
        free_ast_and_or(ast->and_or);
    }

    free(ast);
}
