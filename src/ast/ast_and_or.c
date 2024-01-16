#include <stdlib.h>

#include "ast.h"

struct ast_and_or *new_ast_and_or(void)
{
    struct ast_and_or *new = calloc(1, sizeof(struct ast_and_or));

    new->type = AST_AND_OR;
    new->pipeline = calloc(4, sizeof(struct ast_pipeline *));
    new->op = 0;
    new->size = 4;
    new->pos = 0;

    return new;
}

void add_ast_and_or(struct ast_and_or *ast, struct ast_pipeline *baby)
{
    if (ast->pos > ast->size)
    {
        ast->pipeline =
            realloc(ast->pipeline, (ast->size + 4) * sizeof(struct ast_pipeline *));

        ast->size += 4;
    }

    ast->pipeline[ast->pos] = baby;
    ast->pos++;
}

void print_ast_and_or(struct ast_and_or *ast)
{
    if (!ast)
    {
        return;
    }

    printf("AST_AND_OR\n");
}

void free_ast_and_or(struct ast_and_or *ast)
{
    if (ast->pipeline)
    {
        for (size_t i = 0; i < ast->pos; i++)
        {
            free_ast_pipeline(ast->pipeline[i]);
        }
        free(ast->pipeline);
    }
    free(ast);
}
