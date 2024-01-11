#include <stdlib.h>

#include "ast.h"

struct ast_command *new_ast_command(void)
{
    struct ast_command *new = calloc(1, sizeof(struct ast_command));

    new->type = AST_SIMPLE_COMMAND;
    new->simple_command = NULL;

    return new;
}

void print_ast_command(struct ast_command *ast)
{
    if (!ast)
    {
        return;
    }

    printf("AST_COMMAND\n");

    print_ast_simple_command(ast->simple_command);
}

void free_ast_command(struct ast_command *ast)
{
    if (ast->simple_command)
    {
        free_ast_simple_command(ast->simple_command);
    }

    free(ast);
}
