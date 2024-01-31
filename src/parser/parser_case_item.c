#include "parser.h"

static void pop_and_free(struct lexer *lexer, struct token *tok)
{
    lexer_pop(lexer);
    free(tok->value);
    free_token(tok);
}

static void pop_and_tok(struct lexer *lexer, struct token *tok)
{
    lexer_pop(lexer);
    free_token(tok);
}

static struct token *pop_and_peek(struct lexer *lexer, struct token *tok,
                                  bool *syntax_error)
{
    lexer_pop(lexer);
    free_token(tok);
    tok = lexer_peek(lexer);
    if (!tok)
    {
        *syntax_error = true;
        return NULL;
    }
    return tok;
}

static bool while_pipe(struct lexer *lexer, struct ast_case_item *ast,
                       bool *syntax_error)
{
    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        *syntax_error = true;
        return false;
    }
    while (tok->type == TOKEN_PIPE) // { '|' WORD }
    {
        if (!(tok = pop_and_peek(lexer, tok, syntax_error)))
        {
            return false;
        }

        if (tok->type == TOKEN_WORD)
        {
            add_ast_case_item(ast, tok->value);
        }
        else
        {
            *syntax_error = true;
            return false;
        }

        tok = lexer_peek(lexer);
        if (!tok)
        {
            *syntax_error = true;
            return false;
        }
    }
    return true;
}

struct ast_case_item *parse_case_item(struct lexer *lexer, bool *syntax_error,
                                      int lstage)
{
    struct ast_case_item *ast = new_ast_case_item();
    ast->loop_stage = lstage;
    bool lpar = false;

    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        *syntax_error = true;
        goto error;
    }
    if (tok->type == TOKEN_LPAR)
    {
        lpar = true;
        pop_and_free(lexer, tok);
    }
    tok = lexer_peek(lexer);
    if (!tok)
    {
        *syntax_error = true;
        goto error;
    }
    if (tok->type == TOKEN_WORD)
    {
        add_ast_case_item(ast, tok->value);
        pop_and_tok(lexer, tok);

        if (!while_pipe(lexer, ast, syntax_error))
        {
            goto error;
        }

        tok = lexer_peek(lexer);
        if (!tok || tok->type != TOKEN_RPAR)
        {
            *syntax_error = true;
            goto error;
        }

        pop_and_tok(lexer, tok);
        tok = lexer_peek(lexer);
        if (!tok)
        {
            *syntax_error = true;
            goto error;
        }
        while (tok->type == TOKEN_NEWLINE)
        {
            if (!(tok = pop_and_peek(lexer, tok, syntax_error)))
            {
                goto error;
            }
        }

        ast->compound_list = parse_compound_list(lexer, syntax_error, lstage);

        if (*syntax_error)
        {
            goto error;
        }

        return ast;
    }

    if (lpar)
    {
        *syntax_error = true;
    }

error:
    free_ast_case_item(ast);
    return NULL;
}
