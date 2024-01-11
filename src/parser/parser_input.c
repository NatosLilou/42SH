#include "parser.h"

struct ast_input *parse_input(struct lexer *lexer)
{
    struct ast_input *ast = new_ast_input();

    struct ast_list *baby = parse_list(lexer);
    if (!baby)
    {
        free_ast_input(ast);
        return NULL;
    }

    ast->list = baby;

    if (lexer_peek(lexer).type == TOKEN_NEWLINE)
    {
        lexer_pop(lexer);
        return ast;
    }
    if (lexer_peek(lexer).type == TOKEN_EOF)
    {
        lexer_pop(lexer);
        return ast;
    }

    free_ast_input(ast);
    return NULL;
}
