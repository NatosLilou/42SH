#include <stdlib.h>

#include "ast.h"

struct ast_prefix *new_ast_prefix(void)
{
    struct ast_prefix *new = calloc(1, sizeof(struct ast_prefix));

    new->type = AST_PREFIX;
    new->var = NULL;
    new->redir = NULL;

    return new;
}

void free_ast_prefix(struct ast_prefix *ast)
{
    if (ast)
    {
        if (ast->redir)
        {
            free_ast_redir(ast->redir);
        }

        free(ast);
    }
}
