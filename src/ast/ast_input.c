#include <stdlib.h>

#include "ast.h"

struct ast_input *new_ast_input(void)
{
    struct ast_input *new = calloc(1, sizeof(struct ast_input));

    new->type = AST_INPUT;
    new->list = NULL;

    return new;
}

void print_ast_input(struct ast_input *ast)
{
    if (!ast)
    {
        return;
    }

    printf("AST_INPUT\n");

    print_ast_list(ast->list);
}

void free_ast_input(struct ast_input *ast)
{
    if (ast->list)
    {
        free_ast_list(ast->list);
    }

    free(ast);
}
