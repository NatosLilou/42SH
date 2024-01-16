#include <stdlib.h>

#include "ast.h"

struct ast_redir *new_ast_redir(void)
{
    struct ast_redir *new = calloc(1, sizeof(struct ast_redir));

    new->type = AST_REDIR;
    new->ionumber = 0;
    new->dest = NULL;
    new->redir = TOKEN_ERROR;

    return new;
}

void print_ast_redir(struct ast_redir *ast)
{
    if (!ast)
    {
        return;
    }

    printf("AST_REDIR\n");
}

void free_ast_redir(struct ast_redir *ast)
{
    free(ast);
}
