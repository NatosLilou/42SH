#include <stdlib.h>

#include "ast.h"

struct ast_funcdec *new_ast_funcdec(void)
{
    struct ast_funcdec *new = calloc(1, sizeof(struct ast_funcdec));

    new->type = AST_FUNCDEC;
    new->shell_command = NULL;

    return new;
}

void free_ast_funcdec(struct ast_funcdec *ast)
{
    if (ast)
    {
        if (ast->shell_command)
        {
            free_ast_shell_command(ast->shell_command);
        }
        
        free(ast);
    }
}
// added
