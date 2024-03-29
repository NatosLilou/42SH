#include <stdlib.h>

#include "ast.h"

struct ast_input *new_ast_input(void)
{
    struct ast_input *new = calloc(1, sizeof(struct ast_input));

    new->type = AST_INPUT;
    new->list = NULL;
    new->eof = false;
    new->loop_stage = 0;

    return new;
}

void free_ast_input(struct ast_input *ast)
{
    if (ast)
    {
        if (ast->list)
        {
            free_ast_list(ast->list);
        }

        free(ast);
    }
}
