#include <stdlib.h>

#include "ast.h"

struct ast_simple_command *new_ast_simple_command(char *arg)
{
    struct ast_simple_command *new =
        calloc(1, sizeof(struct ast_simple_command));

    new->type = AST_INPUT;

    char **commands = calloc(4, sizeof(char *));

    new->commands = commands;
    new->size = 3; // Size = len-1 , NULL TERMINATED
    new->pos = 1;

    new->commands[0] = arg;

    return new;
}

void add_ast_simple_command(struct ast_simple_command *ast, char *element)
{
    if (ast->pos >= ast->size)
    {
        new->commands = realloc(
            new->commands, (new->size + 5) * sizeof(char *)); // len+4 => size+5

        new->size += 4;

        for (size_t i = ast->pos + 1; i <= new->size; i++)
        {
            new->commands[i] = '\0';
        }
    }

    ast->commands[ast->pos] = element;
    ast->pos++;
}

void free_ast_simple_command(struct ast_simple_command *ast)
{
    // FREE STRINGS ??
    free(ast->commands);
    free(ast);
}
