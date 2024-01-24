#include "parser.h"

struct ast_input *parse_input(struct lexer *lexer)
{
    bool syntax_error = false;

    struct ast_input *ast = new_ast_input();

    struct ast_list *baby = parse_list(lexer, &syntax_error);
    if (syntax_error)
    {
        goto error;
    }
    if (baby)
    {
        ast->list = baby;
    }

    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        goto error;
    }

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

error:
    free_ast_input(ast);
    return NULL;
}
