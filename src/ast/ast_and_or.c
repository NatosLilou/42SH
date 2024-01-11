#include <stdlib.h>

#include "ast.h"

struct ast_and_or *new_ast_and_or(void)
{
    struct ast_and_or *new = calloc(1, sizeof(struct ast_and_or));

    new->type = AST_AND_OR;
    new->pipeline = NULL;

    return new;
}

void print_ast_and_or(struct ast_and_or *ast)
{
    if (!ast)
    {
        return;
    }

    printf("AST_AND_OR\n");

    print_ast_pipeline(ast->pipeline);
}

void free_ast_and_or(struct ast_and_or *ast)
{
    if (ast->pipeline)
    {
        free_ast_pipeline(ast->pipeline);
    }

    free(ast);
}
