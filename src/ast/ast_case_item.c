#include <stdlib.h>

#include "ast.h"

struct ast_case_item *new_ast_case_item(void)
{
    struct ast_case_item *new = calloc(1, sizeof(struct ast_case_item));

    new->type = AST_CASE_ITEM;

    char **words = calloc(4, sizeof(char *));
    new->words = words;

    new->size = 4;
    new->pos = 0;

    new->compound_list = NULL;

    return new;
}

void add_ast_case_item(struct ast_case_item *ast, char *word)
{
    if (ast->pos >= ast->size)
    {
        ast->words = realloc(ast->words, (ast->size + 4) * sizeof(char *));
        ast->size += 4;
    }

    ast->words[ast->pos] = word;
    ast->pos++;
}

void free_ast_case_item(struct ast_case_item *ast)
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
