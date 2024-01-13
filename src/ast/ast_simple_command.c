#include <stdlib.h>

#include "ast.h"

struct ast_simple_command *new_ast_simple_command(char *arg)
{
    struct ast_simple_command *new =
        calloc(1, sizeof(struct ast_simple_command));

    new->type = AST_SIMPLE_COMMAND;

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
        ast->commands = realloc(
            ast->commands, (ast->size + 5) * sizeof(char *)); // len+4 => size+5

        ast->size += 4;

        for (size_t i = ast->pos + 1; i <= ast->size; i++)
        {
            ast->commands[i] = '\0';
        }
    }

    ast->commands[ast->pos] = element;
    ast->pos++;
}

void print_ast_simple_command(struct ast_simple_command *ast)
{
    if (!ast)
    {
        return;
    }

    printf("AST_SIMPLE_COMMAND: ");

    for (size_t i = 0; i < ast->pos; i++)
    {
        printf("%s ", ast->commands[i]);
    }
    printf("\n");
}

void free_ast_simple_command(struct ast_simple_command *ast)
{
    for (size_t i = 0; i < ast->pos; i++)
    {
        free(ast->commands[i]);
    }
    free(ast->commands);
    free(ast);
}
