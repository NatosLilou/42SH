#include "parser.h"

struct ast_input *parse_input(struct lexer *lexer)
{
    struct ast_input *ast = new_ast_input();

    struct ast_list *baby = parse_list(lexer);
    if (baby)
    {
        ast->list = baby;
    }

    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_NEWLINE)
    {
        tok = lexer_pop(lexer);
        free_token(tok);
        return ast;
    }

    if (tok->type == TOKEN_EOF)
    {
        ast->eof = true;

        tok = lexer_pop(lexer);
        free_token(tok);
        return ast;
    }

    free_ast_input(ast);
    return NULL;
}
