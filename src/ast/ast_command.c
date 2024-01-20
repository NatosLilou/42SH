#include <stdlib.h>

#include "ast.h"

struct ast_command *new_ast_command(void)
{
    struct ast_command *new = calloc(1, sizeof(struct ast_command));

    new->type = AST_COMMAND;
    new->simple_command = NULL;
    new->shell_command = NULL;

    struct ast_redir **redir = calloc(4, sizeof(struct ast_redir *));

    new->redir = redir;
    new->pos = 0;
    new->size = 4;

    return new;
}

void add_ast_command(struct ast_command *ast, struct ast_redir *baby)
{
    if (ast->pos >= ast->size)
    {
        ast->redir =
            realloc(ast->redir, (ast->size + 4) * sizeof(struct redir *));

        ast->size += 4;
    }

    ast->redir[ast->pos] = baby;
    ast->pos++;
}

void free_ast_command(struct ast_command *ast)
{
    if (ast)
    {
        if (ast->simple_command)
        {
            free_ast_simple_command(ast->simple_command);
        }
        if (ast->shell_command)
        {
            free_ast_shell_command(ast->shell_command);
        }

        free(ast);
    }
}
