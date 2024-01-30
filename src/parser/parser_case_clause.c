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

struct ast_case_clause *parse_case_clause(struct lexer *lexer,
                                              bool *syntax_error)
{
    struct ast_case_clause *ast = new_ast_case_clause();

    struct ast_case_item *baby = parse_case_item(lexer, syntax_error);
    if (!baby)
    {
        goto error;
    }
    add_ast_case_clause(ast, baby);

    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        goto error;
    }
    while (tok->type == TOKEN_DSEMI || tok->type == TOKEN_NEWLINE)
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

        struct ast_case_item *baby2 = parse_case_item(lexer, syntax_error);
        if (!baby2)
        {
            return ast;
        }

        add_ast_case_clause(ast, baby2);

        tok = lexer_peek(lexer);
        if (!tok)
        {
            goto error;
        }
    }

    if (tok->type == TOKEN_DSEMI)
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
    free_ast_case_clause(ast);
    return NULL;
}
