#include <stdlib.h>

#include "ast.h"

struct ast_prefix *new_ast_prefix(void)
{
    struct ast_prefix *new = calloc(1, sizeof(struct ast_prefix));

    new->type = AST_PREFIX;
    new->redir = NULL;

    return new;
}

void print_ast_prefix(struct ast_prefix *ast)
{
    if (!ast)
    {
        return;
    }

    printf("AST_PREFIX\n");

    print_ast_redir(ast->redir);
}

void free_ast_prefix(struct ast_prefix *ast)
{
    if (ast->redir)
    {
        free_ast_redir(ast->redir);
    }

    free(ast);
}
