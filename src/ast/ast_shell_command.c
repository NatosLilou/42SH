#include <stdlib.h>

#include "ast.h"

struct ast_shell_command *new_ast_shell_command(void)
{
    struct ast_shell_command *new = calloc(1, sizeof(struct ast_shell_command));

    new->type = AST_SHELL_COMMAND;
    new->rule_if = NULL;

    return new;
}

void print_ast_shell_command(struct ast_shell_command *ast)
{
    if (!ast)
    {
        return;
    }

    printf("AST_SHELL_COMMAND\n");

    print_ast_rule_if(ast->rule_if);
}

void free_ast_shell_command(struct ast_shell_command *ast)
{
    if (ast->rule_if)
    {
        free_ast_rule_if(ast->rule_if);
    }

    free(ast);
}
