#include <stdlib.h>

#include "ast.h"

struct ast_funcdec *new_ast_funcdec(void)
{
    struct ast_funcdec *new = calloc(1, sizeof(struct ast_funcdec));

    new->type = AST_FUNCDEC;
    new->shell_command = NULL;
    new->name = NULL;
    new->loop_stage = 0;

    return new;
}

void free_ast_funcdec(struct ast_funcdec *ast)
{
    if (ast)
    {
        if (ast->name)
        {
            free(ast->name);
        }
        free(ast);
    }
}
