#include <stdlib.h>

#include "ast.h"

struct ast_redir *new_ast_redir(void)
{
    struct ast_redir *new = calloc(1, sizeof(struct ast_redir));

    new->type = AST_REDIR;
    new->ionumber = 1;
    new->dest = NULL;
    new->redir = TOKEN_GREAT;

    return new;
}

void free_ast_redir(struct ast_redir *ast)
{
    if (ast)
    {
        free(ast);
    }
}
