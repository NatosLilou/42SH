#include <stdlib.h>

#include "ast.h"

struct ast_pipeline *new_ast_pipeline(void)
{
    struct ast_pipeline *new = calloc(1, sizeof(struct ast_pipeline));

    new->type = AST_PIPELINE;
    new->command = NULL;

    return new;
}

void free_ast_pipeline(struct ast_pipeline *ast)
{
    if (ast->command)
    {
        free_ast_command(ast->command);
    }

    free(ast);
}
