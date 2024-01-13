#include <stdlib.h>

#include "ast.h"

struct ast_compound_list *new_ast_compound_list(void)
{
    struct ast_compound_list *new = calloc(1, sizeof(struct ast_compound_list));

    new->type = AST_COMPOUND_LIST;

    struct ast_and_or **and_or_list = calloc(4, sizeof(struct ast_and_or *));

    new->and_or = and_or_list;
    new->pos = 0;
    new->size = 4;

    return new;
}

void add_ast_compound_list(struct ast_compound_list *ast,
                           struct ast_and_or *baby)
{
    if (ast->pos >= ast->size)
    {
        ast->and_or =
            realloc(ast->and_or, (ast->size + 4) * sizeof(struct ast_and_or *));

        ast->size += 4;
    }

    ast->and_or[ast->pos] = baby;
    ast->pos++;
}

void print_ast_compound_list(struct ast_compound_list *ast)
{
    if (!ast)
    {
        return;
    }

    printf("AST_COMPOUND_LIST\n");

    for (size_t i = 0; i < ast->pos; i++)
    {
        print_ast_and_or(ast->and_or[i]);
    }
}

void free_ast_compound_list(struct ast_compound_list *ast)
{
    if (ast->and_or)
    {
        for (size_t i = 0; i < ast->pos; i++)
        {
            if (ast->and_or[i])
            {
                free_ast_and_or(ast->and_or[i]);
            }
        }
    }

    free(ast);
}
