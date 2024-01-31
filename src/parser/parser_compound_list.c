#include "parser.h"

static bool newline_loop(struct lexer *lexer, bool *syntax_error)
{
    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        *syntax_error = true;
        return false;
    }

    while (tok->type == TOKEN_NEWLINE)
    {
        lexer_pop(lexer);
        free_token(tok);
        tok = lexer_peek(lexer);
        if (!tok)
        {
            *syntax_error = true;
            return false;
        }
    }

    return true;
}

static bool if_condition(struct token *tok)
{
    return (tok->type == TOKEN_THEN || tok->type == TOKEN_ELIF
            || tok->type == TOKEN_ELSE || tok->type == TOKEN_FI
            || tok->type == TOKEN_DO || tok->type == TOKEN_DONE);
}

struct ast_compound_list *
parse_compound_list(struct lexer *lex, bool *syntax_error, int loop_stage)
{
    struct ast_compound_list *ast = new_ast_compound_list();
    ast->loop_stage = loop_stage;

    if (!newline_loop(lex, syntax_error))
    {
        goto error;
    }

    struct ast_and_or *baby = parse_and_or(lex, syntax_error, loop_stage);
    if (!baby)
    {
        goto error;
    }
    add_ast_compound_list(ast, baby);

    struct token *tok = lexer_peek(lex);
    if (!tok)
    {
        *syntax_error = true;
        goto error;
    }
    while (tok->type == TOKEN_SEMI || tok->type == TOKEN_NEWLINE)
    {
        lexer_pop(lex);
        free_token(tok);

        if (!newline_loop(lex, syntax_error))
        {
            goto error;
        }

        tok = lexer_peek(lex);
        if (!tok)
        {
            *syntax_error = true;
            goto error;
        }
        if (if_condition(tok))
        {
            return ast;
        }

        struct ast_and_or *baby2 = parse_and_or(lex, syntax_error, loop_stage);
        if (!baby2)
        {
            return ast;
        }

        add_ast_compound_list(ast, baby2);

        tok = lexer_peek(lex);
        if (!tok)
        {
            *syntax_error = true;
            goto error;
        }
    }

    if (tok->type == TOKEN_SEMI)
    {
        lexer_pop(lex);
        free_token(tok);
    }

    if (!newline_loop(lex, syntax_error))
    {
        goto error;
    }

    return ast;

error:
    free_ast_compound_list(ast);
    return NULL;
}
