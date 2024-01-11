#include <stdlib.h>

#include "ast.h"

struct ast_list *new_ast_list(void)
{
    struct ast_list *new = calloc(1, sizeof(struct ast_list));

    new->type = AST_LIST;
    new->and_or = NULL;

    return new;
}

void free_ast_list(struct ast_list *ast)
{
    if (ast->and_or)
    {
        free_ast_and_or(ast->and_or);
    }

    free(ast);
}
