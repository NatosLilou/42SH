#include "parser.h"

struct ast_list *parse_list(struct lexer *lexer)
{
    struct ast_list *ast = new_ast_list();

    struct ast_and_or *baby = parse_and_or(lexer);
    if (!baby)
    {
        free_ast_list(ast);
        return NULL;
    }
    add_ast_list(ast, baby);

    while (lexer_peek(lexer).type == TOKEN_SEMI)
    {
        lexer_pop(lexer);

        struct ast_and_or *baby2 = parse_and_or(lexer);
        if (!baby2)
        {
            break;
        }
        add_ast_list(ast, baby2);
    }

    return ast;
}
