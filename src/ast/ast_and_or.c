#include <stdlib.h>

#include "ast.h"

struct ast_and_or *new_ast_and_or(void)
{
    struct ast_and_or *new = calloc(1, sizeof(struct ast_and_or));

    new->type = AST_AND_OR;
    new->pipeline = NULL;

    return new;
}

void free_ast_and_or(struct ast_and_or *ast)
{
    if (ast->pipeline)
    {
        free_ast_pipeline(ast->pipeline);
    }

    free(ast);
}
