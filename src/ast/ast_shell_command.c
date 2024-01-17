#include <stdlib.h>

#include "ast.h"

struct ast_shell_command *new_ast_shell_command(void)
{
    struct ast_shell_command *new = calloc(1, sizeof(struct ast_shell_command));

    new->type = AST_SHELL_COMMAND;
    new->rule_if = NULL;
    new->rule_while = NULL;
    new->rule_until = NULL;
    new->rule_for = NULL;

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
    if (ast)
    {
        if (ast->rule_if)
        {
            free_ast_rule_if(ast->rule_if);
        }
        else if (ast->rule_while)
        {
            free_ast_rule_while(ast->rule_while);
        }
        else if (ast->rule_until)
        {
            free_ast_rule_until(ast->rule_until);
        }
        else if (ast->rule_for)
        {
            free_ast_rule_for(ast->rule_for);
        }

        free(ast);
    }
}
