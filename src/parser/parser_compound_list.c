#include "parser.h"

static bool newline_loop(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        return false;
    }

    while (tok->type == TOKEN_NEWLINE)
    {
        lexer_pop(lexer);
        free_token(tok);
        tok = lexer_peek(lexer);
        if (!tok)
        {
            return false;
        }
    }

    return true;
}

struct ast_compound_list *
parse_compound_list(struct lexer *lexer, bool *syntax_error, int loop_stage)
{
    struct ast_compound_list *ast = new_ast_compound_list();
    ast->loop_stage = loop_stage;

    if (!newline_loop(lexer))
    {
        goto error;
    }

    struct ast_and_or *baby = parse_and_or(lexer, syntax_error, loop_stage);
    if (!baby)
    {
        goto error;
    }
    add_ast_compound_list(ast, baby);

    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        goto error;
    }
    while (tok->type == TOKEN_SEMI || tok->type == TOKEN_NEWLINE)
    {
        lexer_pop(lexer);
        free_token(tok);

        if (!newline_loop(lexer))
        {
            goto error;
        }

        tok = lexer_peek(lexer);
        if (tok->type == TOKEN_THEN || tok->type == TOKEN_ELIF
            || tok->type == TOKEN_ELSE || tok->type == TOKEN_FI
            || tok->type == TOKEN_DO || tok->type == TOKEN_DONE)
        {
            return ast;
        }

        struct ast_and_or *baby2 =
            parse_and_or(lexer, syntax_error, loop_stage);
        if (!baby2)
        {
            return ast;
        }

        add_ast_compound_list(ast, baby2);

        tok = lexer_peek(lexer);
        if (!tok)
        {
            goto error;
        }
    }

    if (tok->type == TOKEN_SEMI)
    {
        lexer_pop(lexer);
        free_token(tok);
    }

    if (!newline_loop(lexer))
    {
        goto error;
    }

    return ast;

error:
    free_ast_compound_list(ast);
    return NULL;
}
