#include <stdlib.h>

#include "ast.h"

struct ast_case_clause *new_ast_case_clause(void)
{
    struct ast_case_clause *new = calloc(1, sizeof(struct ast_case_clause));

    new->type = AST_CASE_CLAUSE;

    struct ast_case_item **items = calloc(4, sizeof(struct ast_case_item *));
    new->case_item = items;

    new->size = 4;
    new->pos = 0;

    return new;
}

void add_ast_case_clause(struct ast_case_clause *ast,
                         struct ast_case_item *item)
{
    if (ast->pos >= ast->size)
    {
        ast->case_item =
            realloc(ast->case_item, (ast->size + 4) * sizeof(char *));
        ast->size += 4;
    }

    ast->case_item[ast->pos] = item;
    ast->pos++;
}

void free_ast_case_clause(struct ast_case_clause *ast)
{
    if (ast)
    {
        if (ast->case_item)
        {
            for (size_t i = 0; i < ast->pos; i++)
            {
                free_ast_case_item(ast->case_item[i]);
            }
            free(ast->case_item);
        }

        free(ast);
    }
}
