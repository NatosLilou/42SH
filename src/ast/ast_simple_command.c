#include <stdlib.h>

#include "ast.h"

struct ast_simple_command *new_ast_simple_command(void)
{
    struct ast_simple_command *new =
        calloc(1, sizeof(struct ast_simple_command));

    new->type = AST_SIMPLE_COMMAND;

    struct ast_prefix **prefix = calloc(4, sizeof(struct ast_prefix *));
    new->prefix = prefix;

    new->size_pref = 4;
    new->pos_pref = 0;

    char **commands = calloc(4, sizeof(char *));
    new->commands = commands;

    new->size_cmd = 3; // NULL TERMINATED
    new->pos_cmd = 0;

    struct ast_redir **redir = calloc(4, sizeof(struct ast_redir *));
    new->redir = redir;

    new->size_redir = 4;
    new->pos_redir = 0;

    new->restore = calloc(1, sizeof(int *));
    new->pos_restore = 0;
    new->loop_stage = 0;
    return new;
}

void add_ast_simple_command_pref(struct ast_simple_command *ast,
                                 struct ast_prefix *prefix)
{
    if (ast->pos_pref >= ast->size_pref)
    {
        ast->size_pref += 4;
        ast->prefix =
            realloc(ast->prefix, ast->size_pref * sizeof(struct ast_prefix *));
    }

    ast->prefix[ast->pos_pref] = prefix;
    ast->pos_pref++;
}

void add_ast_simple_command_cmd(struct ast_simple_command *ast, char *command)
{
    if (ast->pos_cmd >= ast->size_cmd)
    {
        ast->commands =
            realloc(ast->commands, (ast->size_cmd + 5) * sizeof(char *));
        ast->size_cmd += 4;
    }
    for (size_t i = ast->pos_cmd; i < ast->size_cmd + 1; i++)
    {
        ast->commands[i] = NULL;
    }

    ast->commands[ast->pos_cmd] = command;
    ast->pos_cmd++;
}

void add_ast_simple_command_redir(struct ast_simple_command *ast,
                                  struct ast_redir *redir)
{
    if (ast->pos_redir >= ast->size_redir)
    {
        ast->size_redir += 4;
        ast->redir =
            realloc(ast->redir, ast->size_redir * sizeof(struct ast_redir *));
    }

    ast->redir[ast->pos_redir] = redir;
    ast->pos_redir++;
}

void free_ast_simple_command(struct ast_simple_command *ast)
{
    if (ast)
    {
        for (size_t i = 0; i < ast->pos_pref; i++)
        {
            free_ast_prefix(ast->prefix[i]);
        }
        free(ast->prefix);

        for (size_t i = 0; i < ast->pos_cmd; i++)
        {
            free(ast->commands[i]);
        }
        free(ast->commands);

        for (size_t i = 0; i < ast->pos_redir; i++)
        {
            free_ast_redir(ast->redir[i]);
        }
        if (ast->restore)
        {
            for (size_t i = 0; i < ast->pos_restore; i++)
            {
                free(ast->restore[i]);
            }
            free(ast->restore);
        }
        free(ast->redir);

        free(ast);
    }
}
