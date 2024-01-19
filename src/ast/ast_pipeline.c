#include <stdlib.h>

#include "ast.h"

struct ast_pipeline *new_ast_pipeline(void)
{
    struct ast_pipeline *new = calloc(1, sizeof(struct ast_pipeline));

    new->type = AST_PIPELINE;
    new->negation = false;

    new->commands = calloc(4, sizeof(struct ast_command *));

    new->size = 4;
    new->pos = 0;

    return new;
}

void add_ast_pipeline(struct ast_pipeline *ast, struct ast_command *baby)
{
    if (ast->pos >= ast->size)
    {
        ast->commands = realloc(ast->commands,
                                (ast->size + 4) * sizeof(struct ast_command *));

        ast->size += 4;
    }

    ast->commands[ast->pos] = baby;
    ast->pos++;
}

void free_ast_pipeline(struct ast_pipeline *ast)
{
    if (ast)
    {
        if (ast->commands)
        {
            for (size_t i = 0; i < ast->pos; i++)
            {
                free_ast_command(ast->commands[i]);
            }
        }

        free(ast);
    }
}
