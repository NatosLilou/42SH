#include <stdlib.h>

#include "ast.h"

struct ast_rule_for *new_ast_rule_for(void)
{
    struct ast_rule_for *new = calloc(1, sizeof(struct ast_rule_for));

    new->type = AST_RULE_FOR;

    char **words = calloc(4, sizeof(char *));
    new->words = words;

    new->size = 4;
    new->pos = 0;

    new->compound_list = NULL;

    return new;
}

void add_ast_rule_for(struct ast_rule_for *ast, char *word)
{
    if (ast->pos >= ast->size)
    {
        ast->words = realloc(ast->words, (ast->size + 4) * sizeof(char *));
        ast->size += 4;
    }

    ast->words[ast->pos] = word;
    ast->pos++;
}

void free_ast_rule_for(struct ast_rule_for *ast)
{
    if (ast)
    {
        if (ast->words)
        {
            for (size_t i = 0; i < ast->pos; i++)
            {
                free(ast->words[i]);
            }
            free(ast->words);
        }

        free_ast_compound_list(ast->compound_list);
        free(ast);
    }
}
